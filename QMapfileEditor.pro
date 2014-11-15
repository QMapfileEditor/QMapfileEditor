QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug_and_release
TARGET = QMapfileEditor
TEMPLATE = app

QMAKE_CLEAN += $(TARGET)


SOURCES += \
        keyvaluemodel.cpp                      \
        main.cpp                               \
        mainwindow.cpp                         \
        mapscene.cpp                           \
        mapsettings.cpp                        \
        layersettingsvector.cpp                \
        layersettingsraster.cpp                \
        commands/changemapnamecommand.cpp      \
        commands/changemapstatuscommand.cpp    \
        commands/outputformatcommands.cpp      \
        commands/setanglecommand.cpp           \
        commands/setconfigoptioncommand.cpp    \
        commands/setdatapatterncommand.cpp     \
        commands/setdefresolutioncommand.cpp   \
        commands/setfontsetcommand.cpp         \
        commands/setimagecolorcommand.cpp      \
        commands/setmapdebugcommand.cpp        \
        commands/setmapextentcommand.cpp       \
        commands/setmapmaxsizecommand.cpp      \
        commands/setmapprojectioncommand.cpp   \
        commands/setmapsizecommand.cpp         \
        commands/setmapunitscommand.cpp        \
        commands/setmetadatacommand.cpp        \
        commands/setresolutioncommand.cpp      \
        commands/setshapepathcommand.cpp       \
        commands/setsymbolsetcommand.cpp       \
        commands/settemplatepatterncommand.cpp \
        parser/layer.cpp                       \
        parser/mapfileparser.cpp               \
        parser/outputformat.cpp

HEADERS  += \
    keyvaluemodel.h                         \
    mainwindow.h                            \
    mapscene.h                              \
    mapsettings.h                           \
    layersettingsvector.h                   \
    layersettingsraster.h                   \
    commands/changemapnamecommand.h         \
    commands/changemapstatuscommand.h       \
    commands/outputformatcommands.h         \
    commands/setanglecommand.h              \
    commands/setconfigoptioncommand.h       \
    commands/setdatapatterncommand.h        \
    commands/setdefresolutioncommand.h      \
    commands/setfontsetcommand.h            \
    commands/setimagecolorcommand.h         \
    commands/setmapdebugcommand.h           \
    commands/setmapextentcommand.h          \
    commands/setmapmaxsizecommand.h         \
    commands/setmapprojectioncommand.h      \
    commands/setmapsizecommand.h            \
    commands/setmapunitscommand.h           \
    commands/setmetadatacommand.h           \
    commands/setresolutioncommand.h         \
    commands/setshapepathcommand.h          \
    commands/setsymbolsetcommand.h          \
    commands/settemplatepatterncommand.h    \
    parser/layer.h                          \
    parser/mapfileparser.h                  \
    parser/outputformat.h

FORMS    += mainwindow.ui   \
    mapsettings.ui          \
    layersettings.ui        \
    layersettingsvector.ui  \
    layersettingsraster.ui

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
