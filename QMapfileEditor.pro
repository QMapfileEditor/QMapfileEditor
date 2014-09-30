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
    mapsettings.ui \
    layersettings.ui

INCLUDEPATH += "/usr/include/mapserver" \
  "/usr/include/gdal"

LIBS += -lmapserver

RESOURCES += \
    resources.qrc


TRANSLATIONS += lang/fr_FR.ts \
    lang/en_US.ts

coverage.CONFIG += recursive
QMAKE_EXTRA_TARGETS += coverage

CONFIG(debug,debug|release) {

    QMAKE_EXTRA_TARGETS += cov_cxxflags cov_lflags

    cov_cxxflags.target  = coverage
    cov_cxxflags.depends = CXXFLAGS += -fprofile-arcs -ftest-coverage -O0

    cov_lflags.target  = coverage
    cov_lflags.depends = LFLAGS += -fprofile-arcs -ftest-coverage -lgcov

    coverage.commands = @echo "Built with coverage support..."
    build_pass|!debug_and_release:coverage.depends = all

    QMAKE_CLEAN += $(OBJECTS_DIR)*.gcda $(OBJECTS_DIR)*.gcno $(OBJECTS_DIR)*.html
}
