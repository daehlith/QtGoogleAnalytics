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
#include <QtCore/QRegExp>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QtGA/QtGA.h>

const QUrl QtGAConfiguration::SECURE_ENDPOINT = QUrl("https://ssl.google-analytics.com/collect");
const QUrl QtGAConfiguration::NORMAL_ENDPOINT = QUrl("https//www.google-analytics.com/collect");
const QString QtGAConfiguration::USERAGENT = QString("QtGA/1.0");

QtGAConfiguration::QtGAConfiguration()
    : m_trackingID(""), m_endpoint(QtGAConfiguration::NORMAL_ENDPOINT), m_requestType(QtGAConfiguration::DefaultRequest),
      m_userAgent(QtGAConfiguration::USERAGENT), m_cacheBusting(false)
{

}

void QtGAConfiguration::setTrackingID(const QString& trackingID)
{
    QRegExp validTrackingID("\\bUA-\\d{4,10}-\\d{1,4}\\b");
    if ( validTrackingID.exactMatch(trackingID) )
    {
        m_trackingID = trackingID;
    }
    else
    {
        m_trackingID = QString("");
    }
}

const QString& QtGAConfiguration::trackingID() const
{
    return m_trackingID;
}

bool QtGAConfiguration::isSecure() const
{
#ifndef QT_NO_SSL
    return m_endpoint.scheme() == "https";
#else
    return false;
#endif
}

void QtGAConfiguration::setEndpoint(const QUrl &endpoint)
{
#ifdef QT_NO_SSL
    if (endpoint.scheme() == "https")
    {
        m_endpoint = "";
    }
    else
    {
        m_endpoint = endpoint;
    }
#else
    m_endpoint = endpoint;
#endif
}

const QUrl& QtGAConfiguration::endpoint() const
{
    return m_endpoint;
}

void QtGAConfiguration::setIPAnonymization(bool anonymous)
{
    m_anonymizeIP = anonymous;
}

bool QtGAConfiguration::anonymizeIP() const
{
    return m_anonymizeIP;
}

void QtGAConfiguration::setUserAgent(const QString& userAgent)
{
    m_userAgent = userAgent;
}

QString QtGAConfiguration::userAgent() const
{
    return m_userAgent;
}

void QtGAConfiguration::setClientID(const QUuid &clientID)
{
    if ( ! m_clientID.isNull() && clientID.version() == QUuid::Random )
    {
        m_clientID = clientID;
    }
}

QUuid QtGAConfiguration::clientID() const
{
    return m_clientID;
}

QtGA::QtGA(QObject *parent) : QtGA( QtGAConfiguration(), parent)
{

}

QtGA::QtGA(const QtGAConfiguration &config, QObject *parent) : QObject(parent), m_config(config), m_nam(nullptr), m_session(NoSession)
{
    QNetworkAccessManager* obj = qobject_cast<QNetworkAccessManager*>(parent);
    if (obj)
    {
        m_nam = obj;
    }
}

void QtGA::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    if ( manager )
    {
        setParent(manager);
        m_nam = manager;
    }
}

const QNetworkAccessManager* QtGA::networkAccessManager() const
{
    return m_nam;
}

void QtGA::startSession()
{
    m_session = StartSession;
}

void QtGA::endSession()
{
    m_session = EndSession;
}

QtGA::AnalyticsError QtGA::track(HitTypes type, const QVariantMap &parameters, bool interactive)
{
    if ( m_nam == nullptr )
    {
        return ErrorNoNetworkManager;
    }

    QNetworkRequest req(m_config.endpoint());
    req.setHeader(QNetworkRequest::UserAgentHeader, m_config.userAgent());
    QUrlQuery query;
    QByteArray data = query.toString(QUrl::FullyEncoded).toLatin1();
    m_nam->post( req, data );

    return NoError;
}
