#include "registernewcard.h"
#include "ui_registernewcard.h"
#include "user_class.h"
#include "card_class.h"
#include "bank_class.h"
#include "date_class.h"
#include "varieties.h"
#include <string>
#include <qstring.h>
#include <qvector.h>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSignalMapper>
#include <QLineEdit>

int columnNUM = 5;  //每行的元素个数
int selectedBank = -1;  //选择的银行
bool ownerName_flag = false, ownerID_flag = false, ownerPhone_flag = false, payCode_flag = false, payCode_2_flag = false, bank_flag = false;

RegisterNewCard::RegisterNewCard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterNewCard)
{
    ui->setupUi(this);
    initializeUI();
    initializeBankGrid();
}

RegisterNewCard::~RegisterNewCard()
{
    delete ui;
}

void RegisterNewCard::initializeUI()
{
    ui->ownerName->setFocus();
    ui->checkIcon_ownerName->setVisible(false);
    ui->errorText_ownerName->setVisible(false);
    ui->checkIcon_ownerID->setVisible(false);
    ui->errorText_ownerID->setVisible(false);
    ui->checkIcon_ownerPhone->setVisible(false);
    ui->errorText_ownerPhone->setVisible(false);
    ui->checkIcon_payCode->setVisible(false);
    ui->errorText_payCode->setVisible(false);
    ui->checkIcon_payCode_2->setVisible(false);
    ui->errorText_payCode_2->setVisible(false);
}

void RegisterNewCard::initializeBankGrid()
{
    int size = banks.size();    //银行的个数
    QPushButton * bankIconButton[size];   //图标，用QPushButton装载
    signalMapper = new QSignalMapper(this);
    for (int i = 0, k = 0; k < size; i++)
    {
        for (int j = 0; j < columnNUM && k < size; j++, k++)
        {
            bankIconButton[k] = new QPushButton(this);
            bankIconButton[k]->text().clear();  //清除文本
            ui->bankGrid->addWidget(bankIconButton[k], i, j, 1, 1);  //处于第i行第j列，占据1行1列，也就是一格
            bankIconButton[k]->setStyleSheet("QPushButton" + banks[k].get_bankIconURL());
            bankIconButton[k]->setFixedSize(100, 50);    //银行图片长宽比为2:1
            connect(bankIconButton[k], SIGNAL(clicked()), signalMapper, SLOT(map()));
            signalMapper->setMapping(bankIconButton[k], k);
        }
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(selectedBankIcon(int)));
}

void RegisterNewCard::selectedBankIcon(int choose)
{
    ui->selectedBankIcon->setStyleSheet("QLabel" + banks[choose].get_bankIconURL());
    ui->selectedBankName->setText(banks[choose].get_bankName());
    selectedBank = choose;
    bank_flag = true;
}

void RegisterNewCard::on_ownerName_textChanged()
{
    int useless;
    if (!isDigitStr(ui->ownerName->text(), useless, true)) //缺少判断姓名正误的方法
    {
        ui->checkIcon_ownerName->setStyleSheet(errorLabelIcon);
        ui->checkIcon_ownerName->setVisible(true);
        ui->errorText_ownerName->setVisible(true);
        ownerName_flag = false;
    }
    else
    {
        ui->checkIcon_ownerName->setStyleSheet(correctLabelIcon);
        ui->checkIcon_ownerName->setVisible(true);
        ui->errorText_ownerName->setVisible(false);
        ownerName_flag = true;
    }
}

void RegisterNewCard::on_ownerID_textChanged()
{
    int length;
    bool check = isDigitStr(ui->ownerID->text(), length, false);
    if (!check || 18 != length)
    {
        ui->checkIcon_ownerID->setStyleSheet(errorLabelIcon);
        ui->checkIcon_ownerID->setVisible(true);
        ui->errorText_ownerID->setVisible(true);
        ownerID_flag = false;
    }
    else
    {
        ui->checkIcon_ownerID->setStyleSheet(correctLabelIcon);
        ui->checkIcon_ownerID->setVisible(true);
        ui->errorText_ownerID->setVisible(false);
        ownerID_flag = true;
    }
}

void RegisterNewCard::on_ownerPhone_textChanged()
{
    int length;
    bool check = isDigitStr(ui->ownerPhone->text(), length, false);
    if (!check || 11 != length)
    {
        ui->checkIcon_ownerPhone->setStyleSheet(errorLabelIcon);
        ui->checkIcon_ownerPhone->setVisible(true);
        ui->errorText_ownerPhone->setVisible(true);
        ownerPhone_flag = false;
    }
    else
    {
        ui->checkIcon_ownerPhone->setStyleSheet(correctLabelIcon);
        ui->checkIcon_ownerPhone->setVisible(true);
        ui->errorText_ownerPhone->setVisible(false);
        ownerPhone_flag = true;
    }
}

void RegisterNewCard::on_payCode_textChanged()
{
    int length;
    bool check = isDigitStr(ui->payCode->text(), length, false);
    if (!check || 6 != length)
    {
        ui->checkIcon_payCode->setStyleSheet(errorLabelIcon);
        ui->checkIcon_payCode->setVisible(true);
        ui->errorText_payCode->setVisible(true);
        payCode_flag = false;
    }
    else
    {
        ui->checkIcon_payCode->setStyleSheet(correctLabelIcon);
        ui->checkIcon_payCode->setVisible(true);
        ui->errorText_payCode->setVisible(false);
        payCode_flag = true;
    }
}

void RegisterNewCard::on_payCode_2_textChanged()
{
    if (ui->payCode->text() != ui->payCode_2->text())
    {
        ui->checkIcon_payCode_2->setStyleSheet(errorLabelIcon);
        ui->checkIcon_payCode_2->setVisible(true);
        ui->errorText_payCode_2->setVisible(true);
        payCode_2_flag = false;
    }
    else
    {
        ui->checkIcon_payCode_2->setStyleSheet(correctLabelIcon);
        ui->checkIcon_payCode_2->setVisible(true);
        ui->errorText_payCode_2->setVisible(false);
        payCode_2_flag = true;
    }
}

void RegisterNewCard::on_rejectBtn_clicked()
{
    if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("放弃添加银行卡？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        reject();
}

void RegisterNewCard::on_acceptBtn_clicked()
{
    if (!ownerName_flag)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入正确的姓名！"), QMessageBox::Yes);
        ui->ownerName->setFocus();
    }
    else if (!ownerID_flag)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入正确的身份证号！"), QMessageBox::Yes);
        ui->ownerID->setFocus();
    }
    else if (!ownerPhone_flag)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入正确的手机号！"), QMessageBox::Yes);
        ui->ownerPhone->setFocus();
    }
    else if (!payCode_flag)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入正确的6位支付密码！"), QMessageBox::Yes);
        ui->payCode->clear();
        ui->payCode_2->clear();
        ui->payCode->setFocus();
    }
    else if (!payCode_2_flag)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("两次密码不一致！"), QMessageBox::Yes);
        ui->payCode->clear();
        ui->payCode_2->clear();
        ui->payCode->setFocus();
    }
    else if (!bank_flag)
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请选择所属银行！"), QMessageBox::Yes);
    else
    {
        if (QMessageBox::Yes==QMessageBox::warning(this,"Warning",QString::fromLocal8Bit("确认后将不可更改！\n确认添加吗？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes))
        {
            QString cardID = generateCardID(selectedBank, ui->ownerName->text(), ui->ownerPhone->text(), ui->ownerID->text());
            thisUser->addNewCard(cardID);
            cards.push_back(card_CLASS(cardID, ui->payCode->text(), banks[selectedBank].get_bankName(), ui->ownerName->text(), ui->ownerID->text(), ui->ownerPhone->text(), 0));
            cards[cards.size() - 1].addDiary(type_create, 0);
            QMessageBox::information(this, "OK", QString::fromLocal8Bit("添加银行卡成功！"), QMessageBox::Yes);
            accept();
        }
    }
}

QString RegisterNewCard::generateCardID(int selectedBank, QString qName, QString qPhone, QString qID)
{
    QString year, month, day;
    year = QString::number(date.get_year(), 10);
    if (10 <= date.get_month())
        month = QString::number(date.get_month(), 10);
    else
        month = "0" + QString::number(date.get_month(), 10);
    if (10 <= date.get_day())
        day = QString::number(date.get_day(), 10);
    else
        day = "0" + QString::number(date.get_day(), 10);
    return banks[selectedBank].get_bankCode() + getFirstLetter(qName) + qPhone.right(2) + qID.right(2) + year + month + day + banks[selectedBank].get_todayNewCardNUM(true);
}

QString RegisterNewCard::getFirstLetter(QString qName)
{
    std::string strName = qName.toLocal8Bit().data();
    int ucHigh, ucLow, nCode, result = -1;
    ucHigh = (int) (strName.data()[0]&0xFF);
    ucLow = (int) (strName.data()[1]&0xFF);
    if (ucHigh < 0xa1 || ucLow < 0xa1)
    {
        if ('a' <= strName.data()[0] && 'z' >= strName.data()[0])
            result = strName.data()[0] - 'a' + 1;
        else if ('A' <= strName.data()[0] && 'Z' >= strName.data()[0])
            result = strName.data()[0] - 'A' + 1;
    }
    else
    {
        nCode = (ucHigh - 0xa0) * 100 + ucLow - 0xa0;
        result = firstLetter(nCode) - 'A' + 1;
    }
    QString str = QString::number(result, 10);
    if (10 <= result)
        return str;
    else
        return "0" + str;
}

char RegisterNewCard::firstLetter(int nCode)
{
    if (nCode >= 1601 && nCode < 1637) return 'A';
    if (nCode >= 1637 && nCode < 1833) return 'B';
    if (nCode >= 1833 && nCode < 2078) return 'C';
    if (nCode >= 2078 && nCode < 2274) return 'D';
    if (nCode >= 2274 && nCode < 2302) return 'E';
    if (nCode >= 2302 && nCode < 2433) return 'F';
    if (nCode >= 2433 && nCode < 2594) return 'G';
    if (nCode >= 2594 && nCode < 2787) return 'H';
    if (nCode >= 2787 && nCode < 3106) return 'J';
    if (nCode >= 3106 && nCode < 3212) return 'K';
    if (nCode >= 3212 && nCode < 3472) return 'L';
    if (nCode >= 3472 && nCode < 3635) return 'M';
    if (nCode >= 3635 && nCode < 3722) return 'N';
    if (nCode >= 3722 && nCode < 3730) return 'O';
    if (nCode >= 3730 && nCode < 3858) return 'P';
    if (nCode >= 3858 && nCode < 4027) return 'Q';
    if (nCode >= 4027 && nCode < 4086) return 'R';
    if (nCode >= 4086 && nCode < 4390) return 'S';
    if (nCode >= 4390 && nCode < 4558) return 'T';
    if (nCode >= 4558 && nCode < 4684) return 'W';
    if (nCode >= 4684 && nCode < 4925) return 'X';
    if (nCode >= 4925 && nCode < 5249) return 'Y';
    if (nCode >= 5249 && nCode < 5590) return 'Z';
    return '\0';
}

