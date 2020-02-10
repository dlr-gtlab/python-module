#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

BUILD_DEST     = ../../build
MOC_BUILD_DEST = $${BUILD_DEST}

TEMPLATE = app

QT += xml core sql widgets

TARGET = GTlabUnitTest

CONFIG += silent
CONFIG += c++11
CONFIG += console

# Google Test
INCLUDEPATH += $${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/include
win32 {
    CONFIG(debug, debug|release){
        LIBS        += -L$${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/libDebug
        DEPENDPATH  += $${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/libDebug
    } else {
        LIBS        += -L$${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/lib
        DEPENDPATH  += $${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/lib
    }
}
unix {
    LIBS        += -L$${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/lib
    DEPENDPATH  += $${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest/lib
}

CONFIG(debug, debug|release){
    DESTDIR = $${MOC_BUILD_DEST}/debug-unittests
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-unittests/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-unittests/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-unittests/rcc
    UI_DIR = $${MOC_BUILD_DEST}/debug-unittests/ui
} else {
    DESTDIR = $${MOC_BUILD_DEST}/release-unittests
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-unittests/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-unittests/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-unittests/rcc
    UI_DIR = $${MOC_BUILD_DEST}/release-unittests/ui
}

#### INCLUDES
INCLUDEPATH += $${PWD}/../../src

#### HEADERS
HEADERS += $$files(*.h) \

#### SOURCES
SOURCES += $$files(*.cpp)

####################################################

DESTDIR = $${BUILD_DEST}

####################################################

LIBS +=  -lgtest
LIBS += -L../../$${LIB_BUILD_DEST}

####################################################

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
