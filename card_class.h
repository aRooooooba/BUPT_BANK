#ifndef CARD_CLASS_H
#define CARD_CLASS_H

#include <qstring.h>
#include <qvector.h>
#include "diary_class.h"

class card_CLASS
{
private:
    QString cardID;    //卡号
    QString payCode;    //支付密码
    QString bankName;    //所属银行名
    QString ownerName;
    QString ownerID;   //持卡人身份证号
    QString ownerPhone;    //持卡人电话号码
    float money;  //卡内余额
    QVector <diary_CLASS> diary;    //操作纪律

public:
    card_CLASS(QString card_id, QString pay_code, QString bank_name, QString owner_name, QString owner_id, QString owner_phone, float money);
    card_CLASS(){};
    QString get_cardID();
    QString get_payCode();
    QString get_bankName();
    QString get_ownerName();
    QString get_ownerID();
    QString get_ownerPhone();
    float get_money();
    bool change_money(float changement, bool store);
    void editPayCode(QString newPayCode);
    int get_diaryNUM();
    void addDiary(int type, float amount);
    void addDiary(int year, int month, int day, int type, float amount);
    diary_CLASS get_diary(int i);
};

#endif // CARD_CLASS_H
