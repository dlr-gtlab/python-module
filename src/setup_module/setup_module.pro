#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

BUILD_DEST     = ../../$${LIB_BUILD_DEST}
MOC_BUILD_DEST = ../../build


GT_MODULE_ID="Python Setup"

CONFIG(debug, debug|release){
    TARGET = GTlabPythonSetup-d
} else {
    TARGET = GTlabPythonSetup
}

QT += core widgets
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++14

DEFINES += GT_PYTHON_SETUP_DLL


isEmpty(GT_MODULE_ID) {
   error("GT_MODULE_ID undefined. Please define variable GT_MODULE_ID=\"My Module ID\" in project file.")
}

DEFINES += GT_MODULE_ID='"\\\"$${GT_MODULE_ID}\\\""'

CONFIG(debug, debug|release){
    DESTDIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/rcc
    UI_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/ui
} else {
    DESTDIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/rcc
    UI_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/ui
}

INCLUDEPATH += .

DESTDIR = $${BUILD_DEST}

HEADERS += \
    find_libpython.h \
    gt_python_setup.h \
    gtps_globals.h \
    gtps_pythoninterpreter.h \
    gtps_pythonpreferencepage.h \
    gtps_systemsettings.h

SOURCES += \
    gt_python_setup.cpp \
    gtps_globals.cpp \
    gtps_pythoninterpreter.cpp \
    gtps_pythonpreferencepage.cpp \
    gtps_systemsettings.cpp


CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d
    LIBS += -lGTlabGui-d -lGTlabCore-d -lGTlabDataProcessor-d

    # THIRD PARTY
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging
    LIBS += -lGTlabGui -lGTlabCore -lGTlabDataProcessor

    # THIRD PARTY
}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/..\''
}

######################################################################

copyHeaders($$HEADERS)
copyToEnvironmentPathModules()
######################################################################

######################################################################

FORMS += \
    gtps_pythonpreferencepage.ui

RESOURCES += \
    resources.qrc
