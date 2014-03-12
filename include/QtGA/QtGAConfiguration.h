#ifndef QTGACONFIGURATION_H
#define QTGACONFIGURATION_H

#include "QtGA_global.h"

#include <QString>
#include <QUrl>
#include <QUuid>

class QT_GA_EXPORTS QtGAConfiguration
{
public:
    static const QUrl NormalEndpoint;
    static const QUrl SecureEndpoint;
    static const QString UserAgent;

    enum RequestType
    {
        PostRequest = 0,
        GetRequest,
        DefaultRequest = PostRequest,
    };

    enum Version
    {
        QtGA_1_0 = 1,
    };

    QtGAConfiguration();

    void setCacheBusting(bool enabled);
    bool cacheBusting() const;

    void setTrackingID(const QString& trackingID);
    const QString& trackingID() const;

    bool isSecure() const;

    void setEndpoint(const QUrl& endpoint);
    QUrl endpoint() const;

    void setUserAgent(const QString& useragent);
    QString userAgent() const;

    void setIPAnonymization(bool anonymous);
    bool anonymizeIP() const;

    void setClientID(const QUuid& clientID);
    QUuid clientID() const;

private:
    QString m_trackingID;
    QUrl m_endpoint;
    RequestType m_requestType;
    QString m_userAgent;
    bool m_cacheBusting;
    bool m_anonymizeIP;
    QUuid m_clientID;
};

#endif // QTGACONFIGURATION_H
