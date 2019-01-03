#ifndef USER_CLASS_H
#define USER_CLASS_H

#include <qstring.h>
#include <qvector.h>

class user_CLASS
{
private:
    QString username;
    QString password;
    int cardNUM;
    QVector <QString> owningCardsID;

public:
    user_CLASS(QString _username, QString _password);
    user_CLASS(){};
    QString get_username();
    QString get_password();
    int get_cardNUM();
    QString get_owningCardID(int num);
    void addNewCard(QString newCardID);
    void editPassword(QString newPassword);
    void deleteCard(QString cardID);
};

#endif // USER_CLASS_H
