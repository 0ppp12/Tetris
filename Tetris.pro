QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp\
    mainwindow.cpp \
    Item.cpp \
    GameArea.cpp \
    NextArea.cpp

HEADERS  += mainwindow.h \
    Item.h \
    GameArea.h \
    NextArea.h

FORMS    += mainwindow.ui

RESOURCES += \
    rc.qrc

RC_FILE = app.rc
