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

contains(COMPILE_MODE, module|gtlab) {

    INCLUDEPATH += $${DEV_TOOLS}/include
    LIBS        += -L$${DEV_TOOLS}/lib
    DEPENDPATH  += $${DEV_TOOLS}/lib
}

contains(COMPILE_MODE, gtlabRepository|moduleTests) {
    INCLUDEPATH += $${GTLAB_REPO}/src/datamodel \
    $${GTLAB_REPO}/src/datamodel/property \
    $${GTLAB_REPO}/src/utilities/logging \
    $${GTLAB_REPO}/src/core \
    $${GTLAB_REPO}/src/core/settings \
    $${GTLAB_REPO}/src/core/python \
    $${GTLAB_REPO}/src/calculators \
    $${GTLAB_REPO}/src/mdi \
    $${GTLAB_REPO}/src/mdi/tools \
    $${GTLAB_REPO}/src/mdi/post \
    $${GTLAB_REPO}/src/mdi/dock_widgets/process \
    $${GTLAB_REPO}/src/mdi/dock_widgets/process/pages

    LIBS += -L$${GTLAB_REPO}/build
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
