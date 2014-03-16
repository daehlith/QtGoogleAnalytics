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

#ifndef QTGOOGLEANALYTICS_H
#define QTGOOGLEANALYTICS_H

#include "QtGoogleAnalytics_global.h"

#include <QList>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QUrl>

class QNetworkReply;

class QT_GA_EXPORTS QtGoogleAnalyticsTracker : public QObject
{
    Q_OBJECT
public:
    typedef QList<QPair<QString, QString> > ParameterList;
    static const QUrl NormalEndpoint;
    static const QUrl SecureEndpoint;
    static const QString UserAgent;
    static const QString DefaultClientID;
    static const QString ProtocolVersion;

    explicit QtGoogleAnalyticsTracker( QObject* parent=nullptr );

    void setNetworkAccessManager( QNetworkAccessManager* nam );
    QNetworkAccessManager* networkAccessManager() const;

    void track( const QList<QPair<QString, QString> >& parameters );
    void track( const QUrlQuery& data );

    void setTrackingID( const QString& trackingID );
    QString trackingID() const;

    void setUserAgent( const QString& userAgent );
    QString userAgent() const;

    void setEndpoint( const QUrl& endpoint );
    QUrl endpoint() const;

    void setClientID( const QString& clientID );
    QString clientID() const;

    void setOperation( QNetworkAccessManager::Operation op );
    QNetworkAccessManager::Operation operation() const;

    void setCacheBusting( bool enabled );
    bool cacheBusting() const;

signals:
    void tracked();

private slots:
    void onFinished(QNetworkReply* reply);

private:
    void connectSignals();

    QNetworkAccessManager* m_nam;
    QString m_trackingID;
    QString m_userAgent;
    QUrl m_endpoint;
    QString m_clientID;
    QNetworkAccessManager::Operation m_operation;
    bool m_cacheBusting;
};

#endif // QTGOOGLEANALYTICS_H
