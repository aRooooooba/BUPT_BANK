#include <QFile>
#include <QTextStream>
#include "date_class.h"
#include "varieties.h"

date_CLASS::date_CLASS()
{
    QFile dateFile(dateFileAddress);
    if(!dateFile.open(QIODevice::ReadOnly | QIODevice::Text))  //打开文件失败
        exit(0);    //这里无法弹窗报错
    QTextStream input(&dateFile);
    input >> this->year >> this->month >> this->day;
    dateFile.close();
}

date_CLASS::date_CLASS(int _year, int _month, int _day)
{
    this->year = _year;
    this->month = _month;
    this->day = _day;
}

int date_CLASS::get_year()    {return this->year;}

int date_CLASS::get_month()   {return this->month;}

int date_CLASS::get_day() {return this->day;}

void date_CLASS::saveDate()
{
    QFile dateFile(dateFileAddress);
    dateFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream output(&dateFile);
    output << this->year << ' ' << this->month << ' ' << this->day;
    dateFile.close();
}

void date_CLASS::dateCalculator()
{
    this->day++;
    if(32 == this->day && (1 == this->month || 3 == this->month || 5 == this->month || 7 == this->month || 8 == this->month || 10 == this->month || 12 == this->month))
    {   //大月的月末
        this->month++;
        if(13 == this->month)
        {   //年末
            this->month = 1;
            this->year++;
        }
        this->day = 1;
    }
    else if(31 == this->day && (4 == this->month || 6 == this->month || 9 == this->month || 11 == this->month))
    {   //小月的月末
        this->month++;
        this->day = 1;
    }
    else if(2 == this->month)
    {   //2月的月末
        if(30 == this->day && ((0 == this->year % 4 && 0 != this->year % 100) || 0 == this->year % 400))
        {   //闰年的2月
            this->month++;
            this->day = 1;
        }
        else if(29 == this->day)
        {   //平年的2月
            this->month++;
            this->day = 1;
        }
    }
}
