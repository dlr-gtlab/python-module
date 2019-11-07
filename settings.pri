#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

#### LOCAL SETTINGS
include( local_settings.pri )

contains(LINK_DEV_TOOLS, true) {
    message("LINK_DEV_TOOLS = true")
    INCLUDEPATH += $${DEV_TOOLS}/include
    LIBS        += -L$${DEV_TOOLS}/lib
    DEPENDPATH  += $${DEV_TOOLS}/lib
} else {
    INCLUDEPATH += $${GTLAB_PROJECT}/src/datamodel \
    $${GTLAB_PROJECT}/src/datamodel/property \
    $${GTLAB_PROJECT}/src/utilities/logging \
    $${GTLAB_PROJECT}/src/core \
    $${GTLAB_PROJECT}/src/core/settings \
    $${GTLAB_PROJECT}/src/core/python \
    $${GTLAB_PROJECT}/src/calculators \
    $${GTLAB_PROJECT}/src/mdi \
    $${GTLAB_PROJECT}/src/mdi/tools \
    $${GTLAB_PROJECT}/src/mdi/post \
    $${GTLAB_PROJECT}/src/mdi/dock_widgets/process \
    $${GTLAB_PROJECT}/src/mdi/dock_widgets/process/pages

    LIBS += -L$${GTLAB_PROJECT}/build
}

#### THIRD PARTY LIBRARIES
# Python
INCLUDEPATH += $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_373/include
LIBS += -L$${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_373/libs
LIBS        += -L$${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_373
DEPENDPATH  += $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_373
# PythonQt
INCLUDEPATH += $${DEV_TOOLS}/ThirdPartyLibraries/PythonQt/PythonQt_373/include
LIBS        += -L$${DEV_TOOLS}/ThirdPartyLibraries/PythonQt/PythonQt_373/lib
DEPENDPATH  += $${DEV_TOOLS}/ThirdPartyLibraries/PythonQt/PythonQt_373/lib

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
