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
        input >> head;  //"ӵ�еĿ�:"
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
        output << QString::fromLocal8Bit("�û�����") << "       " << iter->get_username() << endl
               << QString::fromLocal8Bit("���룺") << "         " << iter->get_password() << endl
               << QString::fromLocal8Bit("ӵ�п���������") << " " << iter->get_cardNUM() << endl
               << QString::fromLocal8Bit("ӵ�еĿ���") << endl;
        for (int i = 0; i < iter->get_cardNUM(); i++)
            output << QString::fromLocal8Bit("���ţ�") << "         " << iter->get_owningCardID(i) << endl;
        output << "#" << endl;
    }
    output << "$";
    usersFile.close();
}

bool loadCards() //�������п���Ϣ
{
    QFile cardsFile(cardsFileAddress);
    if (!cardsFile.open(QIODevice::ReadOnly | QIODevice::Text)) //���ļ�ʧ��
        return false;
    QTextStream input(&cardsFile);
    QString cardID, payCode, bankName, ownerName, ownerID, ownerPhone, head;
    int diaryNUM, year, month, day, type;
    float amount;
    float money;
    input >> head >> cardID;
    while (head != "$") //���ļ���β���'$'��ʾ����
    {
        input >> head >> payCode
              >> head >> bankName
              >> head >> ownerName
              >> head >> ownerID
              >> head >> ownerPhone
              >> head >> money;
        card_CLASS anotherCard = card_CLASS(cardID, payCode, bankName, ownerName, ownerID, ownerPhone, money);   //�µ�card��Ա
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
        input >> head >> head >> cardID;    //�Ե���#��
    }
    cardsFile.close();
    return true;
}

void saveCards() //�������п���Ϣ
{
    QFile cardsFile(cardsFileAddress);
    cardsFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&cardsFile);
    QVector <card_CLASS>::iterator iter;
    for (iter = cards.begin(); iter != cards.end(); iter++)
    {
        output << QString::fromLocal8Bit("���ţ�") << "           " << iter->get_cardID() << endl
               << QString::fromLocal8Bit("֧�����룺") << "       " << iter->get_payCode() << endl
               << QString::fromLocal8Bit("�������У�") << "       " << iter->get_bankName() << endl
               << QString::fromLocal8Bit("������������") << "     " << iter->get_ownerName() << endl
               << QString::fromLocal8Bit("���������֤�ţ�") << " " << iter->get_ownerID() << endl   //����Ҫ��һ���ո�
               << QString::fromLocal8Bit("�������ֻ��ţ�") << "   " << iter->get_ownerPhone() << endl
               << QString::fromLocal8Bit("��") << "           " << iter->get_money() << endl
               << QString::fromLocal8Bit("��־������") << "       " << iter->get_diaryNUM() << endl;
        for (int i = 0; i < iter->get_diaryNUM(); i++)
            output << QString::fromLocal8Bit("ʱ�䣺") << "           " << iter->get_diary(i).get_happenTime().get_year() << " " << iter->get_diary(i).get_happenTime().get_month() << " " << iter->get_diary(i).get_happenTime().get_day() << endl
                   << QString::fromLocal8Bit("�¼���") << "           " << iter->get_diary(i).get_type() << endl
                   << QString::fromLocal8Bit("��") << "           " << iter->get_diary(i).get_amount() << endl;
        output << "#" << endl;
    }
    output << "$";
    cardsFile.close();
}

bool loadBanks() //����������Ϣ
{
    QFile banksFile(banksFileAddress);
    if (!banksFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream input(&banksFile);
    QString bankName, bankCode, bankIconURL;
    int todayNewCardNUM;
    input >> bankName;
    while (bankName != "$")   //���ļ���β���'$'��ʾ����
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

bool isDigitStr(QString src, int &len, bool mode)  //mode����0���Ƿ������֣�1���Ƿ񲻺�����
{
    len = 0;
    QByteArray ba = src.toLatin1(); //QString ת��Ϊ char*
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
