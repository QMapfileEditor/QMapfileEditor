QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug_and_release
TARGET = QMapfileEditor
TEMPLATE = app

QMAKE_CLEAN += $(TARGET)

SOURCES += \
        keyvaluemodel.cpp                   \
        main.cpp                            \
        mainwindow.cpp                      \
        mapfileparser.cpp                   \
        mapscene.cpp                        \
        mapsettings.cpp                     \
        outputformat.cpp                    \
        commands/changemapnamecommand.cpp   \
        commands/changemapstatuscommand.cpp \
        commands/setmapsizecommand.cpp      \
        commands/setmapmaxsizecommand.cpp   \
        commands/setmapunitscommand.cpp     \
        commands/setmapextentcommand.cpp    \
        commands/setshapepathcommand.cpp    \
        commands/setsymbolsetcommand.cpp

HEADERS  += \
    keyvaluemodel.h                         \
    mainwindow.h                            \
    mapfileparser.h                         \
    mapscene.h                              \
    mapsettings.h                           \
    outputformat.h                          \
    commands/changemapnamecommand.h         \
    commands/changemapstatuscommand.h       \
    commands/setmapsizecommand.h            \
    commands/setmapmaxsizecommand.h         \
    commands/setmapunitscommand.h           \
    commands/setmapextentcommand.h          \
    commands/setshapepathcommand.h          \
    commands/setsymbolsetcommand.h

FORMS    += mainwindow.ui   \
    mapsettings.ui          \
    layersettings.ui

INCLUDEPATH += "/usr/include/mapserver" \
               "/usr/include/gdal"

LIBS += -lmapserver -lgdal

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
