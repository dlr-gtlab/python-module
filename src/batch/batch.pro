#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( ../../settings.pri )

BUILD_DEST     = ../../build
MOC_BUILD_DEST = $${BUILD_DEST}

CONFIG(debug, debug|release){
    TARGET = GTlabPythonConsole$${PY_VERSION}-d
} else {
    TARGET = GTlabPythonConsole$${PY_VERSION}
}

QT += core xml gui widgets
TEMPLATE = app
CONFIG += console
CONFIG += silent
CONFIG += c++11

win32 {
    RC_FILE = batch-script.rc
}

CONFIG(debug, debug|release){
    DESTDIR = $${MOC_BUILD_DEST}/debug-batch-script
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-batch-script/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-batch-script/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-batch-script/rcc
    UI_DIR = $${MOC_BUILD_DEST}/debug-batch-script/ui
} else {
    DESTDIR = $${MOC_BUILD_DEST}/release-batch-script
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-batch-script/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-batch-script/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-batch-script/rcc
    UI_DIR = $${MOC_BUILD_DEST}/release-batch-script/ui
}

INCLUDEPATH += .\
    ../module \
    ../module/utilities

DESTDIR = $${BUILD_DEST}

HEADERS +=

SOURCES += \
    batch-script.cpp

LIBS += -L../../$${LIB_BUILD_DEST}

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabDatamodel-d -lGTlabCalculators-d
    LIBS += -lGTlabCore-d -lGTlabMdi-d

    # GTLAB MODULES
    LIBS += -lGTlabPython$${PY_VERSION}-d

    # THIRD PARTY
} else {
    # GTLAB CORE
    LIBS += -lGTlabDatamodel -lGTlabCalculators
    LIBS += -lGTlabCore -lGTlabMdi

    # GTLAB MODULES
    LIBS += -lGTlabPython$${PY_VERSION}

    # THIRD PARTY
}

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
