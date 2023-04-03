QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 strict_c++

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    acquiredevice.cpp \
    brainwave.cpp \
    decoder.cpp \
    fakestreamer.cpp \
    fbccadecoder.cpp \
    gamepad.cpp \
    psdadecoder.cpp \
    ssveptimer.cpp \
    ssveptetris.cpp \
    stimulu.cpp \
    tetris.cpp

HEADERS += \
    acquiredevice.h \
    brainwave.h \
    decoder.h \
    fakestreamer.h \
    fbccadecoder.h \
    gamepad.h \
    psdadecoder.h \
    ssveptimer.h \
    ssveptetris.h \
    stimulu.h \
    tetris.h

FORMS += \
    ssveptetris.ui

INCLUDEPATH += \
    $$PWD/ThirdParty/brainflow/inc \
    $$PWD/ThirdParty/eigen/include/eigen3

LIBS += \
    -L$$PWD/ThirdParty/brainflow/lib/ \
    -lBrainflow \
    -lDataHandler \
    -lBoardController

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
