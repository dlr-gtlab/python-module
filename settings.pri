#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################
#
#### LOCAL SETTINGS
include( local_settings.pri )

#### GTlab

equals(GTLAB_LIBS, devTools) {
    INCLUDEPATH += $${DEV_TOOLS}/include
    LIBS        += -L$${DEV_TOOLS}/lib
    DEPENDPATH  += $${DEV_TOOLS}/lib
}

equals(GTLAB_LIBS, nightlyBuild) {
    INCLUDEPATH += $${NIGHTLY_BUILD_DIR}/include/core
    INCLUDEPATH += $${NIGHTLY_BUILD_DIR}/include/logging
    LIBS        += -L$${NIGHTLY_BUILD_DIR}/lib
    DEPENDPATH  += $${NIGHTLY_BUILD_DIR}/lib
}

equals(GTLAB_LIBS, repository) {

#### GTlab core
    LIBS        += -L$${GTLAB_CORE}/build
    DEPENDPATH  += $${GTLAB_CORE}/build
    INCLUDEPATH += $${GTLAB_CORE}/src/datamodel \
    $${GTLAB_CORE}/src/datamodel/property \
    $${GTLAB_CORE}/src/utilities/logging \
    $${GTLAB_CORE}/src/core \
    $${GTLAB_CORE}/src/core/settings \
    $${GTLAB_CORE}/src/core/python \
    $${GTLAB_CORE}/src/calculators \
    $${GTLAB_CORE}/src/mdi \
    $${GTLAB_CORE}/src/mdi/tools \
    $${GTLAB_CORE}/src/mdi/post \
    $${GTLAB_CORE}/src/mdi/dock_widgets/process \
    $${GTLAB_CORE}/src/mdi/dock_widgets/process/pages
}

PY_PATH    = $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_$${PY_VERSION}
PY_QT_PATH = $${DEV_TOOLS}/ThirdPartyLibraries/PythonQt/PythonQt_$${PY_VERSION}

#### THIRD PARTY LIBRARIES

# Python
INCLUDEPATH += $${PY_PATH}/include
LIBS += -L$${PY_PATH}/libs
LIBS        += -L$${PY_PATH}
DEPENDPATH  += $${PY_PATH}

# PythonQt
INCLUDEPATH += $${PY_QT_PATH}/include
LIBS        += -L$${PY_QT_PATH}/lib
DEPENDPATH  += $${PY_QT_PATH}/lib

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

######################################################################
