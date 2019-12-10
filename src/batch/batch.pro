#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( ../../settings.pri )

BUILD_DEST_TEMP = ../../build

equals(BUILD_TARGET, applicationDir) {
    BUILD_DEST = $${TARGET_DIRECTORY}
    LIBS += -L$${BUILD_DEST}/modules
    DEPENDPATH  += $${BUILD_DEST}/modules
}

equals(BUILD_TARGET, localDir) {
    BUILD_DEST = $${BUILD_DEST_TEMP}
    LIBS += -L$${BUILD_DEST}
    DEPENDPATH  += $${BUILD_DEST}
}

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
    DESTDIR = $${BUILD_DEST_TEMP}/debug-batch-script
    OBJECTS_DIR = $${BUILD_DEST_TEMP}/debug-batch-script/obj
    MOC_DIR = $${BUILD_DEST_TEMP}/debug-batch-script/moc
    RCC_DIR = $${BUILD_DEST_TEMP}/debug-batch-script/rcc
    UI_DIR = $${BUILD_DEST_TEMP}/debug-batch-script/ui
} else {
    DESTDIR = $${BUILD_DEST_TEMP}/release-batch-script
    OBJECTS_DIR = $${BUILD_DEST_TEMP}/release-batch-script/obj
    MOC_DIR = $${BUILD_DEST_TEMP}/release-batch-script/moc
    RCC_DIR = $${BUILD_DEST_TEMP}/release-batch-script/rcc
    UI_DIR = $${BUILD_DEST_TEMP}/release-batch-script/ui
}

INCLUDEPATH += .\
    ../module \
    ../module/utilities

DESTDIR = $${BUILD_DEST}

HEADERS +=

SOURCES += \
    batch-script.cpp

LIBS += -lGTlabPython$${PY_VERSION} -lGTlabCore -lGTlabMdi

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
