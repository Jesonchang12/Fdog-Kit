﻿#ifndef SSHHANDLE_H
#define SSHHANDLE_H

#include <QObject>
#include "libssh2.h"
#include <libssh2_sftp.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <QDebug>
#include <QTest>
#include <QTextBlock>
#include "datahandle.h"
#include "historycommondwidget.h"
#include <QThread>

#define SSH_PASSWORD 1
#define SSH_SECRET_KEY 2
struct ServerInfoStruct {
    //基础信息
    QString ip;            //ip
    QString runTime;       //运行时间
    QString architecture;  //硬件架构
    QString cpuInfo;        //CPU信息
    QString systemName;    //操作系统名称
    QString loginCount;    //当前登录数

    QString cpuUseRate;    //cpu使用率
    QString memUseRate;    //内存使用率
    QString swapUseRate;   //交换使用率
    QString diskUseRate;   //磁盘使用率

    QString memUse;
    QString swapUse;
    //QString swapFree;

    QString load; //负载

    QString time;          //系统时间
};

Q_DECLARE_METATYPE(ServerInfoStruct);

//class sshHandleThreadEXEC : public QThread {
//    Q_OBJECT
//public:
//    explicit sshHandleThreadEXEC(int connrectType = 0, QString host = "", QString port = "", QString username = "", QString password = "", QObject *parent = nullptr);
//    void run();

//    QString commondExec(QString commond);
//signals:
//    void send_getServerInfo(ServerInfoStruct serverInfo);
//private:
//    LIBSSH2_SESSION *session_exec = nullptr; //exec session
//    LIBSSH2_CHANNEL *channel_exec = nullptr; //exec channel
//    int connrectType;
//    QString host;
//    QString port;
//    QString username;
//    QString password;
//public slots:
//    void getServerInfo();
//};

class sshHandleExec : public QObject {
    Q_OBJECT
public:
    explicit sshHandleExec(QObject *parent = nullptr);
    void getServerInfo();
    QString commondExec(QString commond);
signals:
    void send_getServerInfo(ServerInfoStruct serverInfo);
public slots:
    void init(int connrectType, QString host, QString port, QString username, QString password);
private:
    LIBSSH2_SESSION *session_exec = nullptr; //exec session
    LIBSSH2_CHANNEL *channel_exec = nullptr; //exec channel
    int connrectType;
    QString host;
    QString port;
    QString username;
    QString password;
};

class sshhandle : public QObject
{
    Q_OBJECT
public:
    explicit sshhandle(QObject *parent = nullptr);
    datahandle ac;
    LIBSSH2_SESSION *session_ssh = nullptr;  //ssh session
    LIBSSH2_CHANNEL *channel_ssh = nullptr;  //ssh channel

    LIBSSH2_SESSION *session_exec = nullptr; //exec session
    LIBSSH2_CHANNEL *channel_exec = nullptr; //exec channel

    LIBSSH2_SESSION *       session_ssh_sftp = nullptr;   //exec session
    LIBSSH2_SFTP*           session_sftp = nullptr;       //sftp
    LIBSSH2_SFTP_HANDLE *   handle_sftp = nullptr;        //sftp
signals:
    void send_init();
    void send_init_poll();
    void send_channel_write(QString data);
    void send_channel_read(QString data);
    void send_channel_readS(QStringList data);
    void send_getServerInfo(ServerInfoStruct serverInfo);
public slots:
    //connectAndExecuteCommand(host, port, username, password, command);
    void initSSH(int connrectType, QString host, QString port, QString username, QString password);
    void initEXEC(int connrectType, QString host, QString port, QString username, QString password);
    void initSFTP(int connrectType, QString host, QString port, QString username, QString password);


    void init(int connrectType, QString host, QString port, QString username, QString password);
    void init_poll();
    void channel_write(QString command);
    void channel_read(QString command);
    void getServerInfo();
    void rece_getServerInfo(ServerInfoStruct serverInfo);
    QString commondExec(QString commond);

private:

};

#endif // SSHHANDLE_H
