#ifndef BANK_CLASS_H
#define BANK_CLASS_H

#include <qstring.h>

class bank_CLASS
{
private:
    QString bankName;    //银行名称
    QString bankCode;    //银行代码，用于生成银行卡号
    QString bankIconURL; //存放图标的路径，用于动态图像化
    int todayNewCardNUM;    //记录每天的新卡数量，用于生成卡号

public:
    bank_CLASS(QString bank_name, QString bank_code, QString bank_iconURL, int _todayNewCardNUM);
    bank_CLASS(){};
    QString get_bankName();
    QString get_bankCode();
    QString get_bankIconURL();
    QString get_todayNewCardNUM(bool choose);
    void aNewDay();
};

#endif // BANK_CLASS_H
