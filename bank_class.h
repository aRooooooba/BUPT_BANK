#ifndef BANK_CLASS_H
#define BANK_CLASS_H

#include <qstring.h>

class bank_CLASS
{
private:
    QString bankName;    //��������
    QString bankCode;    //���д��룬�����������п���
    QString bankIconURL; //���ͼ���·�������ڶ�̬ͼ��
    int todayNewCardNUM;    //��¼ÿ����¿��������������ɿ���

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
