#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( ../../settings.pri )

BUILD_DEST = ../../build

TARGET = GTlabPythonConsole$${PY_VERSION}

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

LIBS += -L$${MODULE_BUILD_DEST}
LIBS += -lGTlabPython$${PY_VERSION} -lGTlabCore -lGTlabMdi
LIBS += -lGTlabDatamodel -lGTlabCalculators

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
