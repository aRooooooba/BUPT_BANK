#include <qstring.h>
#include <QMessageBox>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qnetwork.h>
#include "login.h"
#include "ui_login.h"
#include "varieties.h"
#include "user_class.h"
#include "card_class.h"
#include "bank_class.h"
#include "date_class.h"
#include "registernewuser.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->username->setFocus();
    if (!loadUsers())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入用户信息失败！"), QMessageBox::Yes);
        reject();
    }
    if (!loadCards())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入银行卡信息失败！"), QMessageBox::Yes);
        reject();
    }
    if (!loadBanks())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("载入银行信息失败！"), QMessageBox::Yes);
        reject();
    }
    initDate();
    begin = startTimer(1000);
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, 6665))
        close();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptMessage()));
}

void Login::timerEvent(QTimerEvent * event)
{
//    if (event->timerId() == begin && timeFlag)
//    {
//        date.dateCalculator();
        initDate();
//        for (int i = 0; i < (int) banks.size(); i++)
//            banks[i].aNewDay();
//        date.saveDate();
//        saveBanks();
//    }
}

void Login::initDate()
{
    date_CLASS date2 = date;
    date = date_CLASS();
    if (date2.get_day() != date.get_day())
        for (int i = 0; i < (int) banks.size(); i++)
            banks[i].aNewDay();
    ui->date->setText(QString("%1.%2.%3")
                      .arg(QString::number(date.get_year(), 10))
                      .arg(QString::number(date.get_month(), 10))
                      .arg(QString::number(date.get_day(), 10)));
}

void Login::on_registerNewUser_clicked()
{
    killTimer(begin);
    RegisterNewUser r;
    if (Accepted == r.exec())
        saveUsers();
    begin = startTimer(10000);
}

void Login::on_exit_clicked()
{
    saveUsers();
//    date.saveDate();
    reject();
}

void Login::on_login_clicked()
{
    if (ui->username->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入用户名！"), QMessageBox::Yes);
        ui->username->setFocus();
        ui->password->clear();
    }
    else if (ui->password->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入密码！"), QMessageBox::Yes);
        ui->password->setFocus();
    }
    else
    {
        QVector <user_CLASS>::iterator iter;
        for (iter = users.begin(); iter != users.end(); iter++)
        {
            if (iter->get_username() == ui->username->text())
            {
                if (iter->get_password() == ui->password->text())
                {
                    thisUser = iter;
                    ui->username->clear();
                    ui->username->setFocus();
                    ui->password->clear();
                    timeFlag = false;
                    accept();
                }
                else
                {
                    QMessageBox::critical(this, "Error", QString::fromLocal8Bit("密码错误！"), QMessageBox::Yes);
                    ui->password->clear();
                    ui->password->setFocus();
                }
                break;
            }
        }
        if (users.end() == iter)
        {
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("用户名不存在！"), QMessageBox::Yes);
            ui->username->clear();
            ui->username->setFocus();
            ui->password->clear();
        }
    }
}

void Login::acceptMessage()
{
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(sendMessage()));
}

void Login::sendMessage()
{
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_5);
    blockSize = 0;
    if (clientConnection->bytesAvailable() < (int) (2 * sizeof(quint16)))
        return;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    // 设置数据流的版本，客户端和服务器端使用的版本要相同
    out.setVersion(QDataStream::Qt_5_5);
    out << (quint16) 0;
    in >> blockSize;
    if(clientConnection->bytesAvailable() < blockSize)
        return;
    in >> optionType;
    if (0 == (int) optionType)
    {
        in >> TCPusername >> TCPpassword;
        QVector <user_CLASS>::iterator iter;
        for (iter = users.begin(); iter != users.end(); iter++)
            if (TCPusername == iter->get_username() && TCPpassword == iter->get_password())
                break;
        if (users.end() == iter)
            out << (quint16) 0; //失败
        else
        {
            ui->username->setText(TCPusername);
            ui->password->setText(TCPpassword);
            out << (quint16) 1; //成功
        }
        out.device()->seek(0);
        out << (quint16) (block.size() - sizeof(quint16));
        connect(clientConnection, SIGNAL(disconnected()),clientConnection, SLOT(deleteLater()));
        clientConnection->write(block);
        if (users.end() != iter)
            on_login_clicked(); //登录
    }
    else if (1 == (int) optionType)
    {
        in >> TCPusername;
        QVector <user_CLASS>::iterator iter;
        for (iter = users.begin(); iter != users.end(); iter++)
            if (TCPusername == iter->get_username())
                break;
        ui->username->setText(iter->get_username());
        ui->password->setText(iter->get_password());
        connect(clientConnection, SIGNAL(disconnected()),clientConnection, SLOT(deleteLater()));
        on_login_clicked();
    }
}
