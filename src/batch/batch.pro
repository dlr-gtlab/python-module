#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( ../../../../settings.pri )

BUILD_DEST_TEMP = ../../../../build

win32 {
    RC_FILE = batch-script.rc
}

TARGET = GTlabPythonConsole

QT += core xml gui widgets
TEMPLATE = app
CONFIG += console
CONFIG += silent
CONFIG += c++11

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
    ../../../datamodel \
    ../../../calculators \
    ../../../core \
    ../../../mdi \
    ../../../utilities/logging \
    ../../../utilities/numerics \
    ../../../utilities/numerics/bspline \
    ../../../datamodel/property \
    ../src \
    ../src/utilities


DESTDIR = $${BUILD_DEST_TEMP}

HEADERS +=

SOURCES += \
    batch-script.cpp

LIBS += -L$${BUILD_DEST_TEMP} -lGTlabNumerics -lGTlabPhysics -lGTlabLogging -lGTlabDatamodel -lGTlabCalculators -lGTlabCore -lqwt -lGTlabMdi -lGTlabNetwork

LIBS += -L$${DESTDIR}/modules -lGTlabPython

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
