#ifndef REGISTERNEWCARD_H
#define REGISTERNEWCARD_H

#include <QDialog>
#include <QSignalMapper>
#include <qstring.h>
#include <qvector.h>

namespace Ui {
class RegisterNewCard;
}

class RegisterNewCard : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterNewCard(QWidget *parent = 0);
    ~RegisterNewCard();

private:
    Ui::RegisterNewCard *ui;
    QSignalMapper *signalMapper;
    void initializeUI();
    void initializeBankGrid();
    QString generateCardID(int selectedBank, QString qName, QString qPhone, QString qID);
    QString getFirstLetter(QString qName);
    char firstLetter(int nCode);

private slots:
    void selectedBankIcon(int choose);
    void on_ownerID_textChanged();
    void on_ownerName_textChanged();
    void on_ownerPhone_textChanged();
    void on_payCode_textChanged();
    void on_payCode_2_textChanged();
    void on_rejectBtn_clicked();
    void on_acceptBtn_clicked();
};

#endif // REGISTERNEWCARD_H
