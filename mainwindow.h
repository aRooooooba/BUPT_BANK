#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qstring.h>
#include <QTcpSocket>
#include <qnetwork.h>
#include <qtcpserver.h>
class QTcpServer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void closeEvent(QCloseEvent *);

    void on_logout_clicked();

    void locateThisCard(int i);

    void on_editPassword_clicked();

    void on_registerNewCard_clicked();

    void on_storeMoney_clicked();

    void on_takeMoney_clicked();

    void on_deleteCard_clicked();

    void on_editPayCode_clicked();

    void on_history_clicked();

    void on_goback_clicked();

    void sendMessage();

    void acceptMessage();

private:
    Ui::MainWindow *ui;
    QTcpServer * tcpServer;
    QString message;
    quint16 blockSize;  //数据大小信息
    quint16 optionType;
    float totalPrice;
    QTcpSocket * clientConnection;
    void initUI();
    void initCardList();
    void initDiaryList();
    void initCardInformation();
    bool isFloatStr(QString str);
};

#endif // MAINWINDOW_H
