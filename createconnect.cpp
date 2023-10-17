﻿#pragma execution_character_set("utf-8")
#include "createconnect.h"
#include "ui_createconnect.h"

createconnect::createconnect(int8_t connectType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createconnect)
{
    ui->setupUi(this);
    //只显示关闭按钮
    setWindowFlags(Qt::WindowCloseButtonHint);
    //选择显示的连接类型
    this->connectType = connectType;
    if (connectType == SSH_CONNECT_TYPE) {
        ui->stackedWidget->setCurrentIndex(connectType - 1);
        setWindowTitle("新建ssh连接");
    } else if (connectType == ZK_CONNECT_TYPE) {
        ui->stackedWidget->setCurrentIndex(connectType - 1);
        setWindowTitle("新建zookeeper连接");
    } else if (connectType == REDIS_CONNECT_TYPE) {
        ui->stackedWidget->setCurrentIndex(connectType - 1);
        setWindowTitle("新建redis连接");
    } else if (connectType == KAFKA_CONNECT_TYPE) {
        ui->stackedWidget->setCurrentIndex(connectType - 1);
        setWindowTitle("新建kafka连接");
    } else if (connectType == DB_CONNECT_TYPE) {
        ui->stackedWidget->setCurrentIndex(connectType - 1);
        setWindowTitle("新建database连接");
    }
}

createconnect::~createconnect()
{
    delete ui;
}

void createconnect::closeEvent(QCloseEvent *event)
{
    emit newClose();
    this->close();
}

void createconnect::on_toolButton_zk_connect_clicked()
{
    //创建选择的连接信息
    connnectInfoStruct cInfo;
    cInfo.connectType = this->connectType;
    cInfo.name = ui->lineEdit_zk_name->text();
    cInfo.host = ui->lineEdit_zk_host->text();
    cInfo.port = ui->lineEdit_zk_port->text();
    emit newCreate(cInfo);
    this->close();
}

void createconnect::on_toolButton_zk_close_clicked()
{
    emit newClose();
    this->close();
}

void createconnect::on_toolButton_ssh_connect_clicked()
{
    //创建选择的连接信息
    connnectInfoStruct cInfo;
    cInfo.connectType = this->connectType;
    cInfo.name = ui->lineEdit_ssh_name->text();
    cInfo.host = ui->lineEdit_ssh_host->text();
    cInfo.port = ui->lineEdit_ssh_port->text();
    emit newCreate(cInfo);
    this->close();
}

void createconnect::on_toolButton_ssh_close_clicked()
{
    emit newClose();
    this->close();
}

void createconnect::on_widget_bottom_toolButton_connect_clicked()
{
    //创建选择的连接信息
    connnectInfoStruct cInfo;
    cInfo.connectType = 1;//this->connectType;
    cInfo.name = ui->widget_name_lineEdit_name_data->text();
    cInfo.group = ui->widget_group_lineEdit_group_data->text();
    cInfo.host = ui->lineEdit_host_ssh_data->text();
    cInfo.port = ui->lineEdit_port_ssh_data->text();
    cInfo.password = ui->tab_passowrd_lineEdit_password_data->text();
    cInfo.isSavePassword = ui->tab_passowrd_checkBox_remember_password->isChecked();
    emit newCreate(cInfo);
    this->close();
}

void createconnect::on_widget_bottom_toolButton_close_clicked()
{
    emit newClose();
    this->close();
}

void createconnect::on_tab_passowrd_toolButton_show_clicked()
{
    if (isShowPassword) {
       isShowPassword = false;
       //隐藏密码
       ui->tab_passowrd_toolButton_show->setIcon(QIcon(":lib/eye2.png"));
       ui->tab_passowrd_lineEdit_password_data->setEchoMode(QLineEdit::Password);//设置密码隐藏
    } else {
       isShowPassword = true;
       //显示密码
       ui->tab_passowrd_toolButton_show->setIcon(QIcon(":lib/eye1.png"));
       ui->tab_passowrd_lineEdit_password_data->setEchoMode(QLineEdit::Normal);//设置密码显示

    }
}
