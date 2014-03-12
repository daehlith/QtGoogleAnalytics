#include <QtGA/QtGAConfiguration.h>

#include <QtCore/QRegExp>

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
    if ( ( !clientID.isNull() ) && ( clientID.version() == QUuid::Random ) )
    {
        m_clientID = clientID;
    }
}

QUuid QtGAConfiguration::clientID() const
{
    return m_clientID;
}

void QtGAConfiguration::setCacheBusting(bool enabled)
{
    m_cacheBusting = enabled;
}

bool QtGAConfiguration::cacheBusting() const
{
    return m_cacheBusting;
}
