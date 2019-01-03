#ifndef DIARY_CLASS_H
#define DIARY_CLASS_H

#include "date_class.h"

class diary_CLASS
{
private:
    date_CLASS happenTime;
    int type;
    float amount;

public:
    diary_CLASS(){};
    diary_CLASS(int _type, float _amount);
    diary_CLASS(int year, int month, int day, int _type, int _amount);
    date_CLASS get_happenTime();
    int get_type();
    float get_amount();
};

#endif // DIARY_CLASS_H
