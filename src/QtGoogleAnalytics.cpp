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
#include "QtGoogleAnalytics.h"

#include <QtGlobal>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>

const QUrl QtGoogleAnalyticsTracker::NormalEndpoint( "http://www.google-analytics.com/collect" );
const QUrl QtGoogleAnalyticsTracker::SecureEndpoint( "https://ssl.google-analytics.com/collect" );

QtGoogleAnalyticsTracker::QtGoogleAnalyticsTracker( QObject *parent )
    : QObject( parent ), m_nam( new QNetworkAccessManager( this ) )
{
    connectSignals();
}

/*!
 * \brief QtGoogleAnalyticsTracker::setNetworkAccessManager sets the QNetworkAccessManager that should be used by this tracker.
 *
 * Since a tracker requires a valid QNetworkAccessManager instance it will by default construct one of its own. However,
 * for some applications this may not be desirable, and as such the network manager to use can overriden using this method.
 *
 * \note If a nullptr is passed to this function the previously set QNetworkAccessManager will still be used.
 *
 * \sa networkAccessManager()
 */
void QtGoogleAnalyticsTracker::setNetworkAccessManager( QNetworkAccessManager *nam )
{
    if ( ! nam )
    {
        return;
    }

    if ( m_nam && m_nam->parent() == this )
    {
        delete m_nam;
    }
    m_nam = nam;
    connectSignals();
}

QNetworkAccessManager* QtGoogleAnalyticsTracker::networkAccessManager() const
{
    return m_nam;
}

void QtGoogleAnalyticsTracker::track()
{
    QByteArray data;
    QNetworkRequest req;
    QUrl url = NormalEndpoint;
    req.setUrl( url );
    m_nam->post( req, data );
}

void QtGoogleAnalyticsTracker::connectSignals()
{
    connect( m_nam, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( onFinished( QNetworkReply* ) ) );
}

void QtGoogleAnalyticsTracker::onFinished( QNetworkReply *reply )
{
    if ( reply->error() != QNetworkReply::NoError )
    {
        qWarning( reply->errorString().toLocal8Bit() );
    }
    reply->deleteLater();
    emit tracked();
}

void QtGoogleAnalyticsTracker::setTrackingID( const QString& trackingID )
{
    m_trackingID.clear();
    QRegExp validTrackingID( "\\b(UA|YT|MO)-\\d+-\\d+\\b" );
    validTrackingID.setCaseSensitivity( Qt::CaseInsensitive );
    if ( validTrackingID.exactMatch( trackingID ) )
    {
        m_trackingID = trackingID;
    }
}

QString QtGoogleAnalyticsTracker::trackingID() const
{
    return m_trackingID;
}
