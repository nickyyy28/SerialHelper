QT       += core gui
QT += core gui serialport printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT          += charts
CONFIG += c++11

SOURCES += \
    errorpage.cpp \
    errorpage_pid.cpp \
    main.cpp \
    mainwindow.cpp \
    mythread.cpp \
    mywaveplot.cpp \
    qcustomplot.cpp \
    serial.cpp \
    wavedata.cpp

HEADERS += \
    errorpage.h \
    errorpage_pid.h \
    mainwindow.h \
    mythread.h \
    mywaveplot.h \
    qcustomplot.h \
    serial.h \
    wavedata.h

FORMS += \
    errorpage.ui \
    errorpage_pid.ui \
    mainwindow.ui

TRANSLATIONS += \
    HoseComputer_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    logo.rc
