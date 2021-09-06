QT       += core gui
QT += core gui serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT          += charts
CONFIG += c++11


INCLUDEPATH += \
    E:\opencv\build\include \
    D:\Environment\Eigen\include


LIBS += \
    E:\opencv\build\x64\vc15\lib\opencv_world451d.lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    errorpage.cpp \
    main.cpp \
    mainwindow.cpp \
    serial.cpp

HEADERS += \
    errorpage.h \
    mainwindow.h \
    serial.h

FORMS += \
    errorpage.ui \
    mainwindow.ui

TRANSLATIONS += \
    HoseComputer_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
