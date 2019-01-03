#ifndef VARIETIES
#define VARIETIES

#include <qstring.h>
#include <qvector.h>
#include "card_class.h"
#include "date_class.h"
#include "bank_class.h"
#include "user_class.h"

extern QString usersFileAddress, cardsFileAddress, dateFileAddress, banksFileAddress;
extern QString correctLabelIcon, errorLabelIcon;
extern QVector <card_CLASS> cards;
extern QVector <bank_CLASS> banks;
extern QVector <user_CLASS> users;
extern date_CLASS date;
extern user_CLASS * thisUser;
extern bool timeFlag;
extern int type_create, type_editPayCode, type_storeMoney, type_takeMoney, type_pay;

bool loadUsers();
void saveUsers();
bool loadCards();
void saveCards();
bool loadBanks();
void saveBanks();
bool isDigitStr(QString src, int &len, bool mode);

#endif // VARIETIES
