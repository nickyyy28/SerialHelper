QT       += core gui
QT += core gui serialport printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT          += charts
CONFIG += c++11

win32{
INCLUDEPATH += \
    E:\opencv\build\include \
    D:\Environment\Eigen\include
LIBS += \
    E:\opencv\build\x64\vc15\lib\opencv_world451d.lib
}

unix{

}
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    errorpage.cpp \
    errorpage_pid.cpp \
    main.cpp \
    mainwindow.cpp \
    mythread.cpp \
    qcustomplot.cpp \
    serial.cpp \
    wavedata.cpp

HEADERS += \
    errorpage.h \
    errorpage_pid.h \
    mainwindow.h \
    mythread.h \
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
