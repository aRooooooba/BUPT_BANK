#ifndef REGISTERNEWUSER_H
#define REGISTERNEWUSER_H

#include <QDialog>

namespace Ui {
class RegisterNewUser;
}

class RegisterNewUser : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterNewUser(QWidget *parent = 0);
    ~RegisterNewUser();

private slots:
    void on_accept_clicked();

    void on_reject_clicked();

private:
    Ui::RegisterNewUser *ui;
};

#endif // REGISTERNEWUSER_H
