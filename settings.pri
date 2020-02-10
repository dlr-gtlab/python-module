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
include( deployment.pri )

TARGET_DIR_NAME = python$${PY_VERSION}
LIB_BUILD_DEST  = lib/$${TARGET_DIR_NAME}

#### GTlab
LIBS        += -L$${GTLAB_CORE_PATH}/lib/core
DEPENDPATH  += $${GTLAB_CORE_PATH}/lib/core
INCLUDEPATH += $${GTLAB_CORE_PATH}/include/core

LIBS        += -L$${GTLAB_LOGGING_PATH}/lib/logging
DEPENDPATH  += $${GTLAB_LOGGING_PATH}/lib/logging
INCLUDEPATH += $${GTLAB_LOGGING_PATH}/include/logging

#### THIRD PARTY LIBRARIES
# Python
LIBS += -L$${PYTHON_PATH}/libs
LIBS        += -L$${PYTHON_PATH}
DEPENDPATH  += $${PYTHON_PATH}
INCLUDEPATH += $${PYTHON_PATH}/include
unix: INCLUDEPATH += $${PYTHON_PATH}/include/python3.7m

# PythonQt
INCLUDEPATH += $${PYTHON_QT_PATH}/include
LIBS        += -L$${PYTHON_QT_PATH}/lib
DEPENDPATH  += $${PYTHON_QT_PATH}/lib

######################################################################
