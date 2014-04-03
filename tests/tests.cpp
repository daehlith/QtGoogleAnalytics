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

using namespace QtGoogleAnalytics;

TEST(Validation, hitTypeTests)
{
    Tracker::ParameterList params;
    // 1. hit type is required
    EXPECT_FALSE( isValidHit( params ) );
    // 2. Must be one of 'pageview', 'appview', 'event', 'transaction', 'item', 'social', 'exception', 'timing'.
    params << QPair<QString, QString>( "t", "foo" );
    EXPECT_FALSE( isValidHit( params ) );
    // 3. Make sure we succeed on correct parameter
    params.clear();
    params << QPair<QString, QString>( "t", "pageview" );
    EXPECT_TRUE( isValidHit( params ) );
}

TEST(Validation, requiredParameterTests)
{
    Tracker::ParameterList params;
    // 1. check for a hit type that has no required parameters
    params << QPair<QString, QString>( "t", "pageview" );
    EXPECT_TRUE( isValidHit( params ) );
    // 2. detect parameters are missing
    params.clear();
    params << QPair<QString, QString>( "t", "item" );
    EXPECT_FALSE( isValidHit( params ) );
}

TEST(Validation, correctParameterTypeTests)
{
    Tracker::ParameterList params, baseParams;
    // Supported types: Text, Currency, Boolean and Integer
    //  - Text: ... take a guess!
    //  - Currency: Used to represent the total value of a currency.
    //              A decimal point is used as a delimiter between the whole and fractional portion of the currency.
    //              The precision is up to 6 decimal places.
    //              Once the value is sent to Google Analytics, all text is removed up until the first digit,
    //              the - character or the . (decimal) character.
    //              Examples for valid data: 1000.000001, -55.00, $-55.00
    //  - Boolean: 1 - True, 0 - False
    //  - Integer: signed 64 bit integer
    //
    // From that it follows that we only need to test Currency, Boolean and Integer values.

    baseParams << QPair<QString, QString>( "t", "event" );

    // 1. Boolean, valid
    params = baseParams;
    params << QPair<QString, QString>( "aip", "0" );
    params << QPair<QString, QString>( "je", "1" );
    EXPECT_TRUE( isValidHit( params ) );
    // 2. Boolean, invalid
    params = baseParams;
    params << QPair<QString, QString>( "aip", "foo" );
    EXPECT_FALSE( isValidHit( params ) );

    // 3. Currency, valid
    params = baseParams;
    params << QPair<QString, QString>( "tr", "-55.00" );
    params << QPair<QString, QString>( "ts", "1000.000001" );
    EXPECT_TRUE( isValidHit( params ) );
    // 4. Currency, invalid
    params = baseParams;
    params << QPair<QString, QString>( "tt", "foo" );
    EXPECT_FALSE( isValidHit( params ) );

    // 5. Integer, valid
    params = baseParams;
    params << QPair<QString, QString>( "utt", "-1234567890" );
    params << QPair<QString, QString>( "iq", "9876543210" );
    params << QPair<QString, QString>( "ev", "0" );
    EXPECT_TRUE( isValidHit( params ) );
    // 6. Integer, invalid
    params = baseParams;
    params << QPair<QString, QString>( "plt", "foo" );
    EXPECT_FALSE( isValidHit( params ) );
}

TEST(Tracker, setNetworkAccessManager)
{
    // Tests that we can a network manager to use
    TestNetworkAccessManager nam;
    Tracker tracker;
    // 1. network access manager cannot be nulled
    tracker.setNetworkAccessManager( nullptr );
    EXPECT_NE( nullptr, tracker.networkAccessManager() );
    // 2. network access manager can be set to valid values
    tracker.setNetworkAccessManager( &nam );
    EXPECT_EQ( &nam, tracker.networkAccessManager() );
}

TEST(Tracker, trackingID)
{
    Tracker tracker;
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
    Tracker tracker;
    Tracker::ParameterList testParams;
    QString testTrackingID( "UA-0-0" );
    QSignalSpy spy( &tracker, SIGNAL( tracked() ) );
    QUrlQuery expectedData;

    testParams << QPair<QString, QString>( "t", "pageview" );

    expectedPostRequest.setHeader( QNetworkRequest::UserAgentHeader, Tracker::UserAgent );
    expectedPostRequest.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    expectedPostRequest.setUrl( Tracker::NormalEndpoint );

    expectedData.addQueryItem( "t", "pageview" );
    expectedData.addQueryItem( "v", "1" );
    expectedData.addQueryItem( "tid", testTrackingID );
    expectedData.addQueryItem( "cid", Tracker::DefaultClientID );

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
    QUrl tmp = QUrl( Tracker::NormalEndpoint );
    tmp.setQuery( expectedData );

    expectedGetRequest.setUrl( tmp );
    expectedGetRequest.setHeader( QNetworkRequest::UserAgentHeader, Tracker::UserAgent );

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
    QtGoogleAnalytics::Tracker tracker;
    QString expectedUserAgent( "QtGoogleAnalyticsTrackerTests/1.0" );

    // 1. Initialization to default value
    EXPECT_EQ( Tracker::UserAgent, tracker.userAgent() );
    // 2. Illegal user-agent strings do not change the value
    tracker.setUserAgent( "" );
    EXPECT_EQ( Tracker::UserAgent, tracker.userAgent() );
    // 3. Valid user-agent strings are accepted
    tracker.setUserAgent( expectedUserAgent );
    EXPECT_EQ( expectedUserAgent, tracker.userAgent() );
}

TEST(Tracker, endpoint)
{
    Tracker tracker;

    // 1. Initialization to default value
    EXPECT_EQ( Tracker::NormalEndpoint, tracker.endpoint() );
    // 2. Invalid endpoint URL should not change the value
    tracker.setEndpoint( QUrl("") );
    EXPECT_EQ( Tracker::NormalEndpoint, tracker.endpoint() );
    // 3. Valid endpoint URL is accepted
    tracker.setEndpoint( Tracker::SecureEndpoint );
    EXPECT_EQ( Tracker::SecureEndpoint, tracker.endpoint() );
}

TEST(Tracker, clientID)
{
    Tracker tracker;
    QString expectedClientID( "testID" );

    // 1. Initialization
    EXPECT_EQ( Tracker::DefaultClientID, tracker.clientID() );
    // 2. Valid clientIDs are accepted
    tracker.setClientID( expectedClientID );
    EXPECT_EQ( expectedClientID, tracker.clientID() );
    // 3. Invalid should not change value
    tracker.setClientID( "" );
    EXPECT_EQ( expectedClientID, tracker.clientID() );
}

TEST(Tracker, operation)
{
    Tracker tracker;
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
    Tracker tracker;
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
