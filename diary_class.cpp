#include "diary_class.h"
#include "date_class.h"
#include "varieties.h"

diary_CLASS::diary_CLASS(int _type, float _amount)
{
    this->happenTime = date;
    this->type = _type;
    this->amount = _amount;
}

diary_CLASS::diary_CLASS(int year, int month, int day, int _type, int _amount)
{
    this->happenTime = date_CLASS(year, month, day);
    this->type = _type;
    this->amount = _amount;
}

date_CLASS diary_CLASS::get_happenTime()    {return this->happenTime;}

int diary_CLASS::get_type() {return this->type;}

float diary_CLASS::get_amount() {return this->amount;}
