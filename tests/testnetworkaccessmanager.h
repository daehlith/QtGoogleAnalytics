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

#ifndef TESTNETWORKACCESSMANAGER_H
#define TESTNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class QByteArray;
class QNetworkReply;
class QNetworkRequest;
class QIODevice;

class TestNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit TestNetworkAccessManager( QObject *parent = 0 );

    void setExpectedRequest( QNetworkRequest *request );
    void setExpectedData( const QString& data );
    void setExpectedOperation( QNetworkAccessManager::Operation op );

    bool failed() const;

protected:
    virtual QNetworkReply* createRequest( Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0 );

private:
    QNetworkRequest* m_expectedRequest;
    bool m_failed;
    QString m_expectedData;
    QNetworkAccessManager::Operation m_expectedOp;

};

#endif // TESTNETWORKACCESSMANAGER_H
