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


unix {
INCLUDEPATH += \
    /usr/local/include/opencv2  \
    /usr/local/include/opencv
LIBS +=         \
    /usr/local/lib/libopencv_calib3d.so     \
    /usr/local/lib/libopencv_core.so        \
    /usr/local/lib/libopencv_dnn.so         \
    /usr/local/lib/libopencv_features2d.so  \
    /usr/local/lib/libopencv_flann.so       \
    /usr/local/lib/libopencv_highgui.so     \
    /usr/local/lib/libopencv_imgcodecs.so   \
    /usr/local/lib/libopencv_imgproc.so     \
    /usr/local/lib/libopencv_ml.so          \
    /usr/local/lib/libopencv_objdetect.so   \
    /usr/local/lib/libopencv_photo.so       \
    /usr/local/lib/libopencv_shape.so       \
    /usr/local/lib/libopencv_stitching.so   \
    /usr/local/lib/libopencv_superres.so    \
    /usr/local/lib/libopencv_videoio.so     \
    /usr/local/lib/libopencv_video.so       \
    /usr/local/lib/libopencv_videostab.so   \
    /usr/local/lib/libopencv_viz.so         \
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
