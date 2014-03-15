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

#include "testnetworkaccessmanager.h"

#include <QByteArray>
#include <QIODevice>
#include <QNetworkReply>
#include <QNetworkRequest>

TestNetworkAccessManager::TestNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent), m_expectedRequest( nullptr ), m_failed( false )
{
}

QNetworkReply* TestNetworkAccessManager::createRequest( QNetworkAccessManager::Operation op, const QNetworkRequest& request, QIODevice *outgoingData )
{
    QByteArray outgoing = outgoingData->readAll();

    m_failed = ( request != *m_expectedRequest ); // Url, headers or metadata do not match
    m_failed |= ( outgoing != m_expectedData ); // payload mismatch

    return QNetworkAccessManager::createRequest( op, request, outgoingData );
}

void TestNetworkAccessManager::setExpectedRequest( QNetworkRequest *request )
{
    m_expectedRequest = request;
}

void TestNetworkAccessManager::setExpectedData( const QString& data )
{
    m_expectedData = data;
}

bool TestNetworkAccessManager::failed() const
{
    return m_failed;
}
