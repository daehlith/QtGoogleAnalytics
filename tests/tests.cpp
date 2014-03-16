/*
 * Copyright (c) 2014 Thomas Daehling <doc@methedrine.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <QBuffer>
#include <QCoreApplication>
#include <QNetworkRequest>
#include <QSignalSpy>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>
#include <QUuid>

#include <gtest/gtest.h>

#include "../src/QtGoogleAnalytics.h"

#include "testnetworkaccessmanager.h"

TEST(Tracker, setNetworkAccessManager)
{
    // Tests that we can a network manager to use
    TestNetworkAccessManager nam;
    QtGoogleAnalyticsTracker tracker;
    // 1. network access manager cannot be nulled
    tracker.setNetworkAccessManager( nullptr );
    EXPECT_NE( nullptr, tracker.networkAccessManager() );
    // 2. network access manager can be set to valid values
    tracker.setNetworkAccessManager( &nam );
    EXPECT_EQ( &nam, tracker.networkAccessManager() );
}

TEST(Tracker, trackingID)
{
    QtGoogleAnalyticsTracker tracker;
    QString empty;

    // 1. Proper initialization
    EXPECT_EQ( empty, tracker.trackingID() );

    // 2. a bunch of invalid tracking IDs
    tracker.setTrackingID( QString( "WT-1234-56" ) );
    EXPECT_EQ( empty, tracker.trackingID() );
    tracker.setTrackingID( QString( "-1234-56" ) );
    EXPECT_EQ( empty, tracker.trackingID() );
    tracker.setTrackingID( QString( "UA--12" ) );
    EXPECT_EQ( empty, tracker.trackingID() );
    tracker.setTrackingID( QString( "YT-1-" ) );
    EXPECT_EQ( empty, tracker.trackingID() );
    tracker.setTrackingID( QString( "-123-" ) );
    EXPECT_EQ( empty, tracker.trackingID() );
    tracker.setTrackingID( QString( "--1234" ) );

    // 3. a bunch of valid tracking IDs
    QStringList expectedTrackingIDs;
    expectedTrackingIDs << "UA-1234-12" \
                        << "YT-1234-12" \
                        << "MO-1-1" \
                        << "UA-1234-1" \
                        << "ua-1-1";
    Q_FOREACH( QString expected, expectedTrackingIDs )
    {
        tracker.setTrackingID( expected );
        EXPECT_EQ( expected, tracker.trackingID() );
    }
}

TEST(Tracker, track)
{
    // test that we can actually track stuff using QtGoogleAnalytics
    TestNetworkAccessManager nam;
    QNetworkRequest expectedPostRequest;
    QtGoogleAnalyticsTracker tracker;
    QtGoogleAnalyticsTracker::ParameterList testParams;
    QString testTrackingID( "UA-0-0" );
    QSignalSpy spy( &tracker, SIGNAL( tracked() ) );
    QUrlQuery expectedData;

    expectedPostRequest.setHeader( QNetworkRequest::UserAgentHeader, QtGoogleAnalyticsTracker::UserAgent );
    expectedPostRequest.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    expectedPostRequest.setUrl( QtGoogleAnalyticsTracker::NormalEndpoint );

    expectedData.addQueryItem( "v", "1" );
    expectedData.addQueryItem( "tid", testTrackingID );
    expectedData.addQueryItem( "cid", QtGoogleAnalyticsTracker::DefaultClientID );

    nam.setExpectedRequest( &expectedPostRequest );
    nam.setExpectedData( expectedData.toString( QUrl::FullyEncoded ) );

    tracker.setTrackingID( testTrackingID );
    tracker.setNetworkAccessManager( &nam );
    tracker.track( testParams );

    spy.wait();

    EXPECT_EQ( 1, spy.count() );
    EXPECT_FALSE( nam.failed() );

    // GET request
    QNetworkRequest expectedGetRequest;
    QUrl tmp = QUrl( QtGoogleAnalyticsTracker::NormalEndpoint );
    tmp.setQuery( expectedData );

    expectedGetRequest.setUrl( tmp );
    expectedGetRequest.setHeader( QNetworkRequest::UserAgentHeader, QtGoogleAnalyticsTracker::UserAgent );

    nam.setExpectedRequest( &expectedGetRequest );
    nam.setExpectedOperation( QNetworkAccessManager::GetOperation );

    tracker.setOperation( QNetworkAccessManager::GetOperation );
    tracker.track( testParams );

    spy.wait();
    EXPECT_EQ( 2, spy.count() );
    EXPECT_FALSE( nam.failed() );
}

TEST(Tracker, userAgent)
{
    QtGoogleAnalyticsTracker tracker;
    QString expectedUserAgent( "QtGoogleAnalyticsTrackerTests/1.0" );

    // 1. Initialization to default value
    EXPECT_EQ( QtGoogleAnalyticsTracker::UserAgent, tracker.userAgent() );
    // 2. Illegal user-agent strings do not change the value
    tracker.setUserAgent( "" );
    EXPECT_EQ( QtGoogleAnalyticsTracker::UserAgent, tracker.userAgent() );
    // 3. Valid user-agent strings are accepted
    tracker.setUserAgent( expectedUserAgent );
    EXPECT_EQ( expectedUserAgent, tracker.userAgent() );
}

TEST(Tracker, endpoint)
{
    QtGoogleAnalyticsTracker tracker;

    // 1. Initialization to default value
    EXPECT_EQ( QtGoogleAnalyticsTracker::NormalEndpoint, tracker.endpoint() );
    // 2. Invalid endpoint URL should not change the value
    tracker.setEndpoint( QUrl("") );
    EXPECT_EQ( QtGoogleAnalyticsTracker::NormalEndpoint, tracker.endpoint() );
    // 3. Valid endpoint URL is accepted
    tracker.setEndpoint( QtGoogleAnalyticsTracker::SecureEndpoint );
    EXPECT_EQ( QtGoogleAnalyticsTracker::SecureEndpoint, tracker.endpoint() );
}

TEST(Tracker, clientID)
{
    QtGoogleAnalyticsTracker tracker;
    QString expectedClientID( "testID" );

    // 1. Initialization
    EXPECT_EQ( QtGoogleAnalyticsTracker::DefaultClientID, tracker.clientID() );
    // 2. Valid clientIDs are accepted
    tracker.setClientID( expectedClientID );
    EXPECT_EQ( expectedClientID, tracker.clientID() );
    // 3. Invalid should not change value
    tracker.setClientID( "" );
    EXPECT_EQ( expectedClientID, tracker.clientID() );
}

TEST(Tracker, operation)
{
    QtGoogleAnalyticsTracker tracker;
    // 1. Initialization
    EXPECT_EQ( QNetworkAccessManager::PostOperation, tracker.operation() );
    // 2. Unsupported operations are ignored
    tracker.setOperation( QNetworkAccessManager::PutOperation );
    EXPECT_EQ( QNetworkAccessManager::PostOperation, tracker.operation() );
    // 3. Supported operations do work
    tracker.setOperation( QNetworkAccessManager::GetOperation );
    EXPECT_EQ( QNetworkAccessManager::GetOperation, tracker.operation() );
}

TEST(Tracker, cacheBusting)
{
    QtGoogleAnalyticsTracker tracker;
    // 1. Initialization
    EXPECT_FALSE( tracker.cacheBusting() );
    // 2. Setter
    tracker.setCacheBusting( true );
    EXPECT_TRUE( tracker.cacheBusting() );
}

int main(int argc, char** argv)
{
    QCoreApplication app( argc, argv );
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
