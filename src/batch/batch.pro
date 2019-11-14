#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( ../../settings.pri )

win32 {
    RC_FILE = batch-script.rc
}

TARGET = GTlabPythonConsole

QT += core xml gui widgets
TEMPLATE = app
CONFIG += console
CONFIG += silent
CONFIG += c++11

BUILD_DEST_TEMP = ../../build

contains(COMPILE_MODE, module|moduleTests) {
    BUILD_DEST = $${BUILD_DEST_TEMP}
}

equals(COMPILE_MODE, gtlabRepository) {
    BUILD_DEST = $${GTLAB_REPO}/build
    BUILD_DEST_TEMP = $${BUILD_DEST}
}

equals(COMPILE_MODE, gtlab) {
    BUILD_DEST = $${GTLAB_PATH}/bin
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

contains(COMPILE_MODE, gtlabRepository|gtlab) {
    BUILD_DEST = $${BUILD_DEST}/modules
}

LIBS += -L$${BUILD_DEST} -lGTlabNumerics -lGTlabPhysics -lGTlabLogging -lGTlabDatamodel -lGTlabCalculators -lGTlabCore -lGTlabMdi -lGTlabNetwork -lGTlabPython

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
