#include "registernewuser.h"
#include "ui_registernewuser.h"
#include "varieties.h"
#include "user_class.h"
#include <QMessageBox>
#include <qstring.h>
#include <qvector.h>

RegisterNewUser::RegisterNewUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterNewUser)
{
    ui->setupUi(this);
}

RegisterNewUser::~RegisterNewUser()
{
    delete ui;
}

void RegisterNewUser::on_accept_clicked()
{
    if (ui->username->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入用户名！"), QMessageBox::Yes);
        ui->username->setFocus();
    }
    else if (ui->password->text().isEmpty() || ui->password_2->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("请输入密码！"), QMessageBox::Yes);
        ui->password->clear();
        ui->password_2->clear();
        ui->password->setFocus();
    }
    else if (ui->password->text().size() < 6)
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("密码过短，至少要6位！"), QMessageBox::Yes);
        ui->password->clear();
        ui->password_2->clear();
        ui->password->setFocus();
    }
    else if (ui->password->text() != ui->password_2->text())
    {
        QMessageBox::critical(this, "Error", QString::fromLocal8Bit("两次密码不一致！"), QMessageBox::Yes);
        ui->password->clear();
        ui->password_2->clear();
        ui->password->setFocus();
    }
    else
    {
        QVector <user_CLASS>::iterator iter;
        for (iter = users.begin(); iter != users.end(); iter++)
            if (iter->get_username() == ui->username->text())
                break;
        if (users.end() != iter)
        {
            QMessageBox::critical(this, "Error", QString::fromLocal8Bit("用户名已注册过！"), QMessageBox::Yes);
            ui->username->clear();
            ui->username->setFocus();
        }
        else if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确认后将不可更改！\n确认添加吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
        {
            users.append(user_CLASS(ui->username->text(), ui->password->text()));
            QMessageBox::information(this, "OK", QString::fromLocal8Bit("注册成功！"), QMessageBox::Yes);
            accept();
        }
    }
}

void RegisterNewUser::on_reject_clicked()
{
    if (QMessageBox::Yes == QMessageBox::warning(this, "Warning", QString::fromLocal8Bit("确认退出吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
        reject();
}
