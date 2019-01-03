#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "card_class.h"
#include "user_class.h"
#include "date_class.h"
#include "bank_class.h"
#include "varieties.h"
#include "registernewcard.h"
#include <qstring.h>
#include <qvector.h>
#include <QMessageBox>
#include <QInputDialog>
#include <qnetwork.h>

card_CLASS * thisCard = NULL;
bool messageBoxFlag;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    messageBoxFlag = false;
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::LocalHost, 6666))
        close();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
    timeFlag = true;
}

void MainWindow::initUI()
{
    ui->welcomeLabel->setText(QString::fromLocal8Bit("欢迎您，") + thisUser->get_username());
    initCardList();
    initDiaryList();
}

void MainWindow::initCardList()
{
    ui->goback->setVisible(false);
    ui->history->setVisible(false);
    ui->storeMoney->setEnabled(false);
    ui->takeMoney->setEnabled(false);
    ui->deleteCard->setEnabled(false);
    ui->editPayCode->setEnabled(false);
    ui->diaryList->setVisible(false);
    ui->cardList->setColumnWidth(0, 220);
    ui->cardList->setColumnWidth(1, 99);
    ui->cardList->setColumnWidth(2, 120);
    ui->cardList->setColumnWidth(3, 150);
    ui->cardList->setRowCount(thisUser->get_cardNUM());
    for (int i = 0; i < thisUser->get_cardNUM(); i++)
    {
        QVector <card_CLASS>::iterator iterCard;
        for (iterCard = cards.begin(); iterCard != cards.end(); iterCard++)
            if (iterCard->get_cardID() == thisUser->get_owningCardID(i))
                break;
        ui->cardList->setItem(i, 0, new QTableWidgetItem(iterCard->get_cardID()));
        ui->cardList->setItem(i, 1, new QTableWidgetItem(iterCard->get_bankName()));
        ui->cardList->setItem(i, 2, new QTableWidgetItem(iterCard->get_ownerName()));
        ui->cardList->setItem(i, 3, new QTableWidgetItem(iterCard->get_ownerPhone()));
    }
    connect(ui->cardList, SIGNAL(cellClicked(int,int)), this, SLOT(locateThisCard(int)));
}

void MainWindow::initDiaryList()
{
    ui->diaryList->setColumnWidth(0, 150);
    ui->diaryList->setColumnWidth(1, 150);
    ui->diaryList->setColumnWidth(2, 100);
}

void MainWindow::locateThisCard(int i)
{
    QString thisCardID = thisUser->get_owningCardID(i);
    QVector <card_CLASS>::iterator iter;
    for (iter = cards.begin(); iter != cards.end(); iter++)
        if (iter->get_cardID() == thisCardID)
            break;
    thisCard = iter;
    initCardInformation();
}

void MainWindow::initCardInformation()
{
    ui->cardID->setText(thisCard->get_cardID());
    ui->bankName->setText(thisCard->get_bankName());
    ui->ownerName->setText(thisCard->get_ownerName());
    ui->ownerID->setText(thisCard->get_ownerID());
    ui->ownerPhone->setText(thisCard->get_ownerPhone());
    ui->money->setText(QString("%1").arg(thisCard->get_money()));
    ui->history->setVisible(true);
    ui->storeMoney->setEnabled(true);
    ui->takeMoney->setEnabled(true);
    ui->deleteCard->setEnabled(true);
    ui->editPayCode->setEnabled(true);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    on_logout_clicked();
}

void MainWindow::on_logout_clicked()
{
    if (messageBoxFlag || QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("确定注销吗？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        saveCards();
        saveUsers();
        close();
    }
}

void MainWindow::on_editPassword_clicked()
{
    bool ok = false;
    QString oldPassword = QInputDialog::getText(this, "step1", QString::fromLocal8Bit("请输入原始密码："), QLineEdit::Password, QString::null, &ok);
    if (ok)
    {
        if (oldPassword == thisUser->get_password())
        {
            ok = false;
            while (!ok)
            {
                QString newPassword = QInputDialog::getText(this, "step2", QString::fromLocal8Bit("请输入新密码："), QLineEdit::Password, QString::null, &ok);
                if (ok)
                {
                    if (newPassword.size() < 6)
                    {
                        QMessageBox::critical(this,"Error",QString::fromLocal8Bit("密码过短！\n请重新输入！"),QMessageBox::Yes);
                        ok = false;
                    }
                    else
                    {
                        ok = false;
                        QString newPassword2 = QInputDialog::getText(this, "step3", QString::fromLocal8Bit("请重复新密码："), QLineEdit::Password, QString::null, &ok);
                        if (ok)
                        {
                            if (newPassword == newPassword2)
                            {
                                thisUser->editPassword(newPassword);
                                saveUsers();
                                QMessageBox::information(this,"ok",QString::fromLocal8Bit("修改成功，请牢记密码！"),QMessageBox::Yes);
                            }
                            else
                            {
                                QMessageBox::critical(this,"Error",QString::fromLocal8Bit("两次密码不一致！\n请重新输入！"),QMessageBox::Yes);
                                ok = false;
                            }
                        }
                        else
                            break;
                    }
                }
                else
                    break;
            }
        }
        else
            QMessageBox::critical(this,"Error",QString::fromLocal8Bit("原始密码错误！"),QMessageBox::Yes);
    }
}

void MainWindow::on_registerNewCard_clicked()
{
    RegisterNewCard r;
    if (QDialog::Accepted == r.exec())
        initCardList();
}

void MainWindow::on_storeMoney_clicked()
{
    bool ok = false;
    while (!ok)
    {
        QString money = QInputDialog::getText(this, QString::fromLocal8Bit("存款"), QString::fromLocal8Bit("请输入存款金额："), QLineEdit::Normal, QString::null, &ok);
        if (ok && !money.isEmpty())
        {
            if (isFloatStr(money))
            {
                thisCard->change_money(money.toFloat(), true);
                thisCard->addDiary(type_storeMoney, money.toFloat());
                QMessageBox::information(this,"OK",QString::fromLocal8Bit("存钱成功！"),QMessageBox::Yes);
                initCardInformation();
            }
            else
            {
                QMessageBox::critical(this,"Error",QString::fromLocal8Bit("请输入正确的存钱金额！"),QMessageBox::Yes);
                ok = false;
            }
        }
    }
}

void MainWindow::on_takeMoney_clicked()
{
    bool ok = false;
    while (!ok)
    {
        QString money = QInputDialog::getText(this, QString::fromLocal8Bit("取款"), QString::fromLocal8Bit("请输入取款金额："), QLineEdit::Normal, QString::null, &ok);
        if (ok && !money.isEmpty())
        {
            if (isFloatStr(money))
            {
                ok = false;
                while (!ok)
                {
                    QString payCode = QInputDialog::getText(this, QString::fromLocal8Bit("取款"), QString::fromLocal8Bit("请输入取款密码："), QLineEdit::Password, QString::null, &ok);
                    if (ok)
                    {
                        if (payCode == thisCard->get_payCode())
                        {
                            if (thisCard->change_money(money.toFloat(), false))
                            {
                                thisCard->addDiary(type_takeMoney, money.toFloat());
                                QMessageBox::information(this,QString::fromLocal8Bit("OK"),QString::fromLocal8Bit("取钱成功！"),QMessageBox::Yes);
                                initCardInformation();
                            }
                            else
                                QMessageBox::critical(this,QString::fromLocal8Bit("Error"),QString::fromLocal8Bit("余额不足！"),QMessageBox::Yes);
                        }
                        else
                        {
                            QMessageBox::critical(this,"Error",QString::fromLocal8Bit("密码错误！"),QMessageBox::Yes);
                            ok = false;
                        }
                    }
                    else
                        ok = true;
                }
            }
            else
            {
                QMessageBox::critical(this,"Error",QString::fromLocal8Bit("请输入正确的存钱金额！"),QMessageBox::Yes);
                ok = false;
            }
        }
        else
            ok = true;
    }
}

bool MainWindow::isFloatStr(QString str)
{
    QByteArray ba = str.toLatin1(); //QString 转换为 char*
    const char *s = ba.data();
    while (*s && ((*s >= '0' && *s <= '9') || *s == '.'))
        s++;
    if (*s)
        return false;
    else
        return true;
}

void MainWindow::on_deleteCard_clicked()
{
    if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("销卡后不可返还余额！\n请确认卡里余额已经取完！"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
    {
        bool ok = false;
        QString password = QInputDialog::getText(this, QString::fromLocal8Bit("销卡"), QString::fromLocal8Bit("请输入登录密码："), QLineEdit::Password, QString::null, &ok);
        if (ok)
        {
            if (password == thisUser->get_password())
            {
                ok = false;
                QString payCode = QInputDialog::getText(this, QString::fromLocal8Bit("销卡"), QString::fromLocal8Bit("请输入取款密码："), QLineEdit::Password, QString::null, &ok);
                if (ok)
                {
                    if (payCode == thisCard->get_payCode())
                    {
                        thisUser->deleteCard(thisCard->get_cardID());
                        cards.erase(thisCard);
                        thisCard = NULL;
                        QMessageBox::information(this,QString::fromLocal8Bit("取款"),QString::fromLocal8Bit("销卡成功！"),QMessageBox::Yes);
                        initCardList();
                    }
                    else
                        QMessageBox::critical(this,"Error",QString::fromLocal8Bit("支付密码错误！"),QMessageBox::Yes);
                }
            }
            else
                QMessageBox::critical(this,"Error",QString::fromLocal8Bit("登录密码错误！"),QMessageBox::Yes);
        }
    }
}

void MainWindow::on_editPayCode_clicked()
{
    bool ok = false;
    while (!ok)
    {
        QString oldPayCode = QInputDialog::getText(this, "step1", QString::fromLocal8Bit("请输入原支付密码："), QLineEdit::Password, QString::null, &ok);
        if (ok && !oldPayCode.isEmpty())
        {
            if (oldPayCode == thisCard->get_payCode())
            {
                ok = false;
                while (!ok)
                {
                    QString newPayCode1 = QInputDialog::getText(this, "step2", QString::fromLocal8Bit("请输入新支付密码："), QLineEdit::Password, QString::null, &ok);
                    if (ok)
                    {
                        int len;
                        if (isDigitStr(newPayCode1, len, false) && 6 == len)
                        {
                            ok = false;
                            QString newPayCode2 = QInputDialog::getText(this, "step3", QString::fromLocal8Bit("请重复新支付密码："), QLineEdit::Password, QString::null, &ok);
                            if (ok)
                            {
                                if (newPayCode1 == newPayCode2)
                                {
                                    thisCard->editPayCode(newPayCode1);
                                    thisCard->addDiary(type_editPayCode, 0);
                                    QMessageBox::information(this, "OK",QString::fromLocal8Bit("支付密码修改成功！"),QMessageBox::Yes);
                                }
                                else
                                {
                                    QMessageBox::critical(this,"Error",QString::fromLocal8Bit("两次密码不一致！"),QMessageBox::Yes);
                                    ok = false;
                                }
                            }
                            else
                                ok = true;
                        }
                        else
                        {
                            QMessageBox::critical(this,"Error",QString::fromLocal8Bit("请输入正确的6位数字支付密码！"),QMessageBox::Yes);
                            ok = false;
                        }
                    }
                    else
                        ok = true;
                }
            }
            else
            {
                QMessageBox::critical(this,"Error",QString::fromLocal8Bit("原支付密码错误！"),QMessageBox::Yes);
                ok = false;
            }
        }
        else
            ok = true;
    }
}

void MainWindow::on_history_clicked()
{
    ui->diaryList->setVisible(true);
    ui->cardList->setVisible(false);
    ui->history->setVisible(false);
    ui->goback->setVisible(true);
    ui->diaryList->setRowCount(thisCard->get_diaryNUM());
    for (int i = 0; i < thisCard->get_diaryNUM(); i++)
    {
        diary_CLASS anotherDiary = thisCard->get_diary(i);
        ui->diaryList->setItem(i, 0, new QTableWidgetItem(QString::number(anotherDiary.get_happenTime().get_year(), 10) + "." +
                                                          QString::number(anotherDiary.get_happenTime().get_month(), 10) + "." +
                                                          QString::number(anotherDiary.get_happenTime().get_day())));
        switch (anotherDiary.get_type()) {
        case 0:
            ui->diaryList->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("创建")));
            break;
        case 1:
            ui->diaryList->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("修改支付密码")));
            break;
        case 2:
            ui->diaryList->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("存款")));
            ui->diaryList->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(anotherDiary.get_amount())));
            break;
        case 3:
            ui->diaryList->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("取款")));
            ui->diaryList->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(anotherDiary.get_amount())));
            break;
        case 4:
            ui->diaryList->setItem(i, 1, new QTableWidgetItem(QString::fromLocal8Bit("支出")));
            ui->diaryList->setItem(i, 2, new QTableWidgetItem(QString("%1").arg(anotherDiary.get_amount())));
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_goback_clicked()
{
    ui->diaryList->setVisible(false);
    ui->cardList->setVisible(true);
    ui->history->setVisible(true);
    ui->goback->setVisible(false);
}

void MainWindow::acceptMessage()
{
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(sendMessage()));
}

void MainWindow::sendMessage()
{
    QDataStream in(clientConnection);
    // 设置数据流版本，这里要和服务器端相同
    in.setVersion(QDataStream::Qt_5_5);
    blockSize = 0;
    if (clientConnection->bytesAvailable() < (int) (2 * sizeof(quint16)))
        return;
    in >> blockSize;
    if (clientConnection->bytesAvailable() < blockSize)
        return;
    in >> optionType;
    if (0 == (int) optionType || 1 == (int) optionType)
    {
        messageBoxFlag = true;
        on_logout_clicked();
    }
    else if (2 == (int) optionType)
    {
        QString tp;
        in >> tp;
        totalPrice = tp.toFloat();
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << (quint16) 0;
        out << (quint16) 2;
        for (int i = 0; i < thisUser->get_cardNUM(); i++)
        {
            QVector <card_CLASS>::iterator iter;
            for (iter = cards.begin(); iter != cards.end(); iter++)
                if (thisUser->get_owningCardID(i) == iter->get_cardID())
                    break;
            if (iter->get_money() >= totalPrice)
                out << iter->get_cardID();
        }
        out.device()->seek(0);
        out << (quint16) (block.size() - sizeof(quint16));
        clientConnection->write(block);
    }
    else if (3 == (int) optionType)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        out << (quint16) 0;
        QString cardID, payCode;
        in >> cardID >> payCode;
        QVector <card_CLASS>::iterator iter;
        for (iter = cards.begin(); iter != cards.end(); iter++)
            if (cardID == iter->get_cardID())
                break;
        if (payCode == iter->get_payCode())
        {
            iter->change_money(totalPrice, false);
            iter->addDiary(type_pay, totalPrice);
            saveCards();
            out << (quint16) 3;
        }
        else
            out << (quint16) 4;
        out.device()->seek(0);
        out << (quint16) (block.size() - sizeof(quint16));
        clientConnection->write(block);
    }
    connect(clientConnection, SIGNAL(disconnected()),clientConnection, SLOT(deleteLater()));
}
