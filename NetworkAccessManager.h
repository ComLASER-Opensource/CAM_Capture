/**
 * @file NetworkAccessManager.h
 
 * @brief wifi 서치를 수행하는 클래스
 * @version 0.1
 * @date 2023-12-06
 * 

 * 
 */

#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QObject>
#include <QNetworkConfiguration>

/**
 * @brief wifi 서치를 수행하는 클래스
 * 
 */
class NetworkAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkAccessManager(QObject *parent = nullptr);
    int foundCount;
    QNetworkConfiguration netcfg;
    QStringList WiFisList;
    QList<QNetworkConfiguration> netcfgList;
    QStringList findActiveWirelesses();
signals:

};

#endif // NETWORKACCESSMANAGER_H
