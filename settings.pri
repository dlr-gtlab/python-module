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

INCLUDEPATH += $${DEV_TOOLS}/include
LIBS        += -L$${DEV_TOOLS}/lib
DEPENDPATH  += $${DEV_TOOLS}/lib

#### THIRD PARTY LIBRARIES
# Python
INCLUDEPATH += $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_373/include
LIBS += $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_373/libs/python37.lib
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

#

######################################################################
