#-------------------------------------------------
#
# Project created by QtCreator 2022-05-21T15:04:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Plotting
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    drawing.cpp \
    rtplot.cpp \
    qplot.cpp \
    rtthread.cpp

HEADERS += \
        mainwindow.h \
    drawing.h \
    rtplot.h \
    qplot.h \
    plotstruct.h \
    ../include/DataIPC.h \
    ../include/IPCStruct.h \
    rtthread.h

FORMS += \
        mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/library/Simd/release/ -lSimd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/library/Simd/debug/ -lSimd
else:unix:!macx: LIBS += -L$$PWD/library/Simd/ -lSimd

INCLUDEPATH += $$PWD/library/Simd/src
DEPENDPATH += $$PWD/library/Simd/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/library/Simd/release/libSimd.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/library/Simd/debug/libSimd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/library/Simd/release/Simd.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/library/Simd/debug/Simd.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/library/Simd/libSimd.a

QMAKE_CXXFLAGS += -mavx2
