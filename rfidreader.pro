#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T11:50:11
#
#-------------------------------------------------

QT       += core gui svg network

TARGET = rfidreader
TEMPLATE = app


SOURCES += main.cpp\
        rfidtask.cpp\
        configwindow.cpp

HEADERS  += rfidtask.h\
         configwindow.h

FORMS    += configwindow.ui

INCLUDEPATH += ./include/
win32{
	LIBS += -L"$$_PRO_FILE_PWD_/lib/" -llibnfc
} else {
	LIBS += -L"$$_PRO_FILE_PWD_/lib/" -lnfc
}

RESOURCES += \
    icon.qrc
