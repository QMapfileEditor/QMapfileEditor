TEMPLATE = app
TARGET = qgisimporter
INCLUDEPATH += .
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += "/usr/include/mapserver" \
               "/usr/include/gdal"

LIBS += -lmapserver -lgdal


CONFIG += console debug_and_release

QMAKE_CLEAN += $(TARGET)

# Input
HEADERS += qgisimporter.h ../mapfileparser.h ../outputformat.h
SOURCES += main.cpp qgisimporter.cpp ../mapfileparser.cpp ../outputformat.cpp


