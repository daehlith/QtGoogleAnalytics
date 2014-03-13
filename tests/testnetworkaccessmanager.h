#ifndef TESTNETWORKACCESSMANAGER_H
#define TESTNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class TestNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit TestNetworkAccessManager(QObject *parent = 0);

signals:

public slots:

};

#endif // TESTNETWORKACCESSMANAGER_H
