#-------------------------------------------------
#
# Project created by QtCreator 2017-10-07T10:20:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BUPT_BANK + tcpServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bank_class.cpp \
    card_class.cpp \
    date_class.cpp \
    varieties.cpp \
    login.cpp \
    registernewcard.cpp \
    user_class.cpp \
    registernewuser.cpp \
    diary_class.cpp

HEADERS  += mainwindow.h \
    bank_class.h \
    card_class.h \
    date_class.h \
    varieties.h \
    login.h \
    registernewcard.h \
    user_class.h \
    registernewuser.h \
    diary_class.h

FORMS    += mainwindow.ui \
    login.ui \
    registernewcard.ui \
    registernewuser.ui

RESOURCES += \
    pics/bankIcon/bankicon.qrc \
    pics/labelIcon/labelicon.qrc \
    pics/background/background.qrc \
    pics/buttonIcon/buttonicon.qrc
