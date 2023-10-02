QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/kiss_fft

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    $$PWD/thirdparty/kissfft/kiss_fft.c \
    workers/spectrogramWorker.cpp

HEADERS += \
    mainwindow.h \
    $$PWD/thirdparty/kissfft/kiss_fft.h \
    workers/spectrogramWorker.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += sndfile

INCLUDEPATH += /usr/include/opencv4

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs


RESOURCES += \
    thirdparty/opencv/modules/highgui/src/window_QT.qrc
