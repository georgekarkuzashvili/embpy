#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T13:33:02
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = embpy
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/include/python2.7 \
    -I/usr/include/x86_64-linux-gnu/python2.7

CCFLAG += -fno-strict-aliasing \
    -D_FORTIFY_SOURCE=2 \
    -g -fstack-protector-strong -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O2 -Wall -Wstrict-prototypes

LIBS += -lpython2.7 -lpthread -ldl  -lutil -lm

SOURCES += main.cpp

OTHER_FILES += \
    func.py