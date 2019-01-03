#include <qstring.h>
#include <fstream>
#include <qvector.h>
#include <QFile>
#include <QTextStream>
#include "varieties.h"
#include "user_class.h"
#include "card_class.h"
#include "date_class.h"
#include "bank_class.h"
#include "diary_class.h"

QString usersFileAddress = "D:\\BUPT_BANK\\txt\\usersFile.txt";
QString cardsFileAddress = "D:\\BUPT_BANK\\txt\\cardsFile.txt";
QString dateFileAddress = "D:\\BUPT_BANK\\txt\\dateFile.txt";
QString banksFileAddress = "D:\\BUPT_BANK\\txt\\banksFile.txt";
QString correctLabelIcon = "QLabel{border-image:url(:/correct.png)}";
QString errorLabelIcon = "QLabel{border-image:url(:/error.png)}";
QVector <card_CLASS> cards;
QVector <bank_CLASS> banks;
QVector <user_CLASS> users;
date_CLASS date = date_CLASS();
user_CLASS * thisUser = NULL;
bool timeFlag = true;
int type_create = 0;
int type_editPayCode = 1;
int type_storeMoney = 2;
int type_takeMoney = 3;
int type_pay = 4;

bool loadUsers()
{
    QFile usersFile(usersFileAddress);
    if (!usersFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&usersFile);
    QString username, password, cardID, head;
    int cardNUM;
    input >> head >> username;
    while (head != "$")
    {
        input >> head >> password
              >> head >> cardNUM;
        user_CLASS anotherUser = user_CLASS(username, password);
        input >> head;  //"拥有的卡:"
        for (int i = 0; i < cardNUM; i++)
        {
            input >> head >> cardID;
            anotherUser.addNewCard(cardID);
        }
        users.append(anotherUser);
        input >> head >> head >> username;
    }
    usersFile.close();
    return true;
}

void saveUsers()
{
    QFile usersFile(usersFileAddress);
    usersFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&usersFile);
    QVector <user_CLASS>::iterator iter;
    for (iter = users.begin(); iter != users.end(); iter++)
    {
        output << QString::fromLocal8Bit("用户名：") << "       " << iter->get_username() << endl
               << QString::fromLocal8Bit("密码：") << "         " << iter->get_password() << endl
               << QString::fromLocal8Bit("拥有卡的数量：") << " " << iter->get_cardNUM() << endl
               << QString::fromLocal8Bit("拥有的卡：") << endl;
        for (int i = 0; i < iter->get_cardNUM(); i++)
            output << QString::fromLocal8Bit("卡号：") << "         " << iter->get_owningCardID(i) << endl;
        output << "#" << endl;
    }
    output << "$";
    usersFile.close();
}

bool loadCards() //载入银行卡信息
{
    QFile cardsFile(cardsFileAddress);
    if (!cardsFile.open(QIODevice::ReadOnly | QIODevice::Text)) //打开文件失败
        return false;
    QTextStream input(&cardsFile);
    QString cardID, payCode, bankName, ownerName, ownerID, ownerPhone, head;
    int diaryNUM, year, month, day, type;
    float amount;
    float money;
    input >> head >> cardID;
    while (head != "$") //在文件结尾添加'$'以示结束
    {
        input >> head >> payCode
              >> head >> bankName
              >> head >> ownerName
              >> head >> ownerID
              >> head >> ownerPhone
              >> head >> money;
        card_CLASS anotherCard = card_CLASS(cardID, payCode, bankName, ownerName, ownerID, ownerPhone, money);   //新的card成员
        input >> head >> diaryNUM;
        for (int i = 0; i < diaryNUM; i++)
        {
            input >> head >> year >> month >> day
                  >> head >> type
                  >> head >> amount;
            if (year < date.get_year() && month <= date.get_month() && day <= date.get_day())
                continue;
            anotherCard.addDiary(year, month, day, type, amount);
        }
        cards.push_back(anotherCard);
        input >> head >> head >> cardID;    //吃掉“#”
    }
    cardsFile.close();
    return true;
}

void saveCards() //保存银行卡信息
{
    QFile cardsFile(cardsFileAddress);
    cardsFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&cardsFile);
    QVector <card_CLASS>::iterator iter;
    for (iter = cards.begin(); iter != cards.end(); iter++)
    {
        output << QString::fromLocal8Bit("卡号：") << "           " << iter->get_cardID() << endl
               << QString::fromLocal8Bit("支付密码：") << "       " << iter->get_payCode() << endl
               << QString::fromLocal8Bit("所属银行：") << "       " << iter->get_bankName() << endl
               << QString::fromLocal8Bit("所有者姓名：") << "     " << iter->get_ownerName() << endl
               << QString::fromLocal8Bit("所有者身份证号：") << " " << iter->get_ownerID() << endl   //至少要有一个空格
               << QString::fromLocal8Bit("所有者手机号：") << "   " << iter->get_ownerPhone() << endl
               << QString::fromLocal8Bit("余额：") << "           " << iter->get_money() << endl
               << QString::fromLocal8Bit("日志数量：") << "       " << iter->get_diaryNUM() << endl;
        for (int i = 0; i < iter->get_diaryNUM(); i++)
            output << QString::fromLocal8Bit("时间：") << "           " << iter->get_diary(i).get_happenTime().get_year() << " " << iter->get_diary(i).get_happenTime().get_month() << " " << iter->get_diary(i).get_happenTime().get_day() << endl
                   << QString::fromLocal8Bit("事件：") << "           " << iter->get_diary(i).get_type() << endl
                   << QString::fromLocal8Bit("金额：") << "           " << iter->get_diary(i).get_amount() << endl;
        output << "#" << endl;
    }
    output << "$";
    cardsFile.close();
}

bool loadBanks() //载入银行信息
{
    QFile banksFile(banksFileAddress);
    if (!banksFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&banksFile);
    QString bankName, bankCode, bankIconURL;
    int todayNewCardNUM;
    input >> bankName;
    while (bankName != "$")   //在文件结尾添加'$'以示结束
    {
        input >> bankCode >> bankIconURL >> todayNewCardNUM;
        banks.append(bank_CLASS(bankName, bankCode, bankIconURL, todayNewCardNUM));
        input >> bankName;
    }
    banksFile.close();
    return true;
}

void saveBanks()
{
    QFile banksFile(banksFileAddress);
    banksFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&banksFile);
    QVector <bank_CLASS>::iterator iter;
    for (iter = banks.begin(); iter != banks.end(); iter++)
        output << iter->get_bankName() << " "
               << iter->get_bankCode() << " "
               << iter->get_bankIconURL() << " "
               << iter->get_todayNewCardNUM(false) << endl;
    output << "$";
    banksFile.close();
}

bool isDigitStr(QString src, int &len, bool mode)  //mode——0：是否都是数字；1：是否不含数字
{
    len = 0;
    QByteArray ba = src.toLatin1(); //QString 转换为 char*
    const char *s = ba.data();
    if (false == mode)
    {
        while (*s && *s >= '0' && *s <= '9')
        {
            s++;
            len++;
        }
    }
    else
    {
        while (*s && (*s < '0' || *s > '9'))
        {
            s++;
            len++;
        }
    }
    if (*s)
        return false;
    else
        return true;
}
