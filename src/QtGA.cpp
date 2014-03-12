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
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QtGA/QtGA.h>

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
