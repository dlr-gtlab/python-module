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

CONFIG(debug, debug|release){
    TARGET = GTlabPythonSetup$${PY_VERSION}-d
} else {
    TARGET = GTlabPythonSetup$${PY_VERSION}
}

QT += core widgets
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++11

DEFINES += GT_PYTHON_SETUP_DLL

DEFINES += "GT_MODULE_ID=\"Python Setup\""

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
    gt_python_setup.h

SOURCES += \
    gt_python_setup.cpp

CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d

    greaterThan(MAJOR_VERSION, 1) {
        message(GTlab Version 2 or newer)
        LIBS += -lGTlabCore-d -lGTlabDataProcessor-d
    } else {
        message(GTlab Version older than 2)
        # GTLAB CORE
        LIBS += -lGTlabCore-d
    }

    # THIRD PARTY
    win32: LIBS += -lpython$${PY_MAJOR_VERSION}$${PY_MINOR_VERSION}_d

    unix {
        LIBS += -lpython$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}d
        #equals(PY_VERSION, 37) {
        #    DEFINES += PYTHON_LIBRARY=\\\"libpython3.7m.so.1.0\\\"
        #} else {
        #    DEFINES += PYTHON_LIBRARY=\\\"libpython$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}d.so.1.0\\\"
        #}
    }
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging

    greaterThan(MAJOR_VERSION, 1) {
        message(GTlab Version 2 or newer)
        LIBS += -lGTlabCore -lGTlabDataProcessor
    } else {
        message(GTlab Version older than 2)
        # GTLAB CORE
        LIBS += -lGTlabCore
    }

    # GTLAB MODULES

    # THIRD PARTY
    win32: LIBS += -lpython$${PY_MAJOR_VERSION}$${PY_MINOR_VERSION}
    unix {
        LIBS += -lpython$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}

        #equals(PY_VERSION, 37) {

        #    DEFINES += PYTHON_LIBRARY=\\\"libpython3.7m.so.1.0\\\"
        #} else {

        #    DEFINES += PYTHON_LIBRARY=\\\"libpython$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}.so.1.0\\\"
        #}
   }
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
