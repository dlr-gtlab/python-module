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

#### THIRD PARTY LIBRARIES
equals(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 12) {
    message(Qt Version 5.12 or newer)

        # Google Test
        INCLUDEPATH += $${GOOGLE_TEST_PATH}/include
        LIBS        += -L$${GOOGLE_TEST_PATH}/lib
        DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib

} else {
    message(Qt Version older than 5.12)

        # Google Test
        INCLUDEPATH += $${GOOGLE_TEST_PATH}/include
        win32 {
                CONFIG(debug, debug|release){
                        LIBS        += -L$${GOOGLE_TEST_PATH}/libDebug
                        DEPENDPATH  += $${GOOGLE_TEST_PATH}/libDebug
                } else {
                        LIBS        += -L$${GOOGLE_TEST_PATH}/lib
                        DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib
                }
        }
        unix {
                LIBS        += -L$${GOOGLE_TEST_PATH}/lib
                DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib
        }
}

######################################################################
