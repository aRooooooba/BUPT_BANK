#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <qtcpserver.h>
#include <qnetwork.h>
class QTcpServer;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);

private slots:
    void on_registerNewUser_clicked();

    void on_exit_clicked();

    void on_login_clicked();

    void sendMessage();

    void acceptMessage();

private:
    Ui::Login *ui;
    QTcpServer * tcpServer;
    QString TCPusername;
    QString TCPpassword;
    quint16 blockSize;
    quint16 optionType;
    QTcpSocket * clientConnection;
    void initDate();

protected:
    void timerEvent(QTimerEvent * event);
    int begin;
};

#endif // LOGIN_H
