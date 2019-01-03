#include "user_class.h"
#include <qstring.h>
#include <qvector.h>

user_CLASS::user_CLASS(QString _username, QString _password)
{
    this->username = _username;
    this->password = _password;
    this->cardNUM = 0;
}

QString user_CLASS::get_username()  {return this->username;}

QString user_CLASS::get_password()  {return this->password;}

int user_CLASS::get_cardNUM()   {return this->owningCardsID.size();}

QString user_CLASS::get_owningCardID(int num)  {return this->owningCardsID[num];}

void user_CLASS::addNewCard(QString newCardID)
{
    this->owningCardsID.push_back(newCardID);
    this->cardNUM++;
}

void user_CLASS::editPassword(QString newPassword)
{
    this->password = newPassword;
}

void user_CLASS::deleteCard(QString cardID)
{
    QVector <QString>::iterator iter;
    for (iter = this->owningCardsID.begin(); *iter != cardID; iter++);
    this->owningCardsID.erase(iter);
    this->cardNUM--;
}
