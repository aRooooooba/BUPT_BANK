#include <qstring.h>
#include "card_class.h"

card_CLASS::card_CLASS(QString card_id, QString pay_code, QString bank_name, QString owner_name, QString owner_id, QString owner_phone, float money)
{
    this->cardID = card_id;
    this->payCode = pay_code;
    this->bankName = bank_name;
    this->ownerName = owner_name;
    this->ownerID = owner_id;
    this->ownerPhone = owner_phone;
    this->money = money;
}

QString card_CLASS::get_cardID()   {return this->cardID;}

QString card_CLASS::get_payCode()  {return this->payCode;}

QString card_CLASS::get_bankName() {return this->bankName;}

QString card_CLASS::get_ownerName()  {return this->ownerName;}

QString card_CLASS::get_ownerID()  {return this->ownerID;}

QString card_CLASS::get_ownerPhone()   {return this->ownerPhone;}

float card_CLASS::get_money() {return this->money;}

bool card_CLASS::change_money(float changement, bool store)
{
    if (true == store)
    {
        this->money += changement;
        return true;
    }
    else
    {
        if (changement > this->money)
            return false;
        this->money -= changement;
        return true;
    }
}

void card_CLASS::editPayCode(QString newPayCode)
{
    this->payCode = newPayCode;
}

int card_CLASS::get_diaryNUM()
{
    return this->diary.size();
}

void card_CLASS::addDiary(int type, float amount)
{
    this->diary.push_back(diary_CLASS(type, amount));
}

void card_CLASS::addDiary(int year, int month, int day, int type, float amount)
{
    this->diary.push_back(diary_CLASS(year, month, day, type, amount));
}

diary_CLASS card_CLASS::get_diary(int i)    {return this->diary[i];}
