#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T17:22:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug_and_release
TARGET = QMapfileEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        mapsettings.cpp \
        mapfileparser.cpp

HEADERS  += mainwindow.h \
    mapsettings.h \
    mapfileparser.h

FORMS    += mainwindow.ui \
    mapsettings.ui

INCLUDEPATH += "/usr/include/mapserver" \
  "/usr/include/gdal"

LIBS += -lmapserver

RESOURCES += \
    resources.qrc

lupdate.commands = lupdate QMapfileEditor.pro
lupdate.depends  = $$SOURCES $$HEADERS $$FORMS $$TRANSLATIONS
lrelease.commands = lrelease QMapfileEditor.pro
lrelease.depends = lupdate
QMAKE_EXTRA_TARGETS += lupdate lrelease


TRANSLATIONS += lang/fr.ts \
    lang/en.ts
