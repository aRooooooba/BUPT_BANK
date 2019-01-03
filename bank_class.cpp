#include <qstring.h>
#include "bank_class.h"

bank_CLASS::bank_CLASS(QString bank_name, QString bank_code, QString bank_iconURL, int _todayNewCardNUM)
{
    this->bankName = bank_name;
    this->bankCode = bank_code;
    this->bankIconURL = bank_iconURL;
    this->todayNewCardNUM = _todayNewCardNUM;
}

QString bank_CLASS::get_bankName() {return this->bankName;}

QString bank_CLASS::get_bankCode() {return this->bankCode;}

QString bank_CLASS::get_bankIconURL()    {return this->bankIconURL;}

QString bank_CLASS::get_todayNewCardNUM(bool choose)
{
    QString result;
    if (choose)
        this->todayNewCardNUM++;
    result = QString::number(this->todayNewCardNUM, 10);
    if (10 <= this->todayNewCardNUM)
        return result;
    else
        return "0" + result;

}

void bank_CLASS::aNewDay()
{
    this->todayNewCardNUM = 0;
}
