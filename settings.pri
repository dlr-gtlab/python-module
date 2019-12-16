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

TARGET_DIR_NAME = python$${PY_VERSION}

MODULE_BUILD_DEST = ../../lib/$${TARGET_DIR_NAME}
MOC_BUILD_DEST = ../../build

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

# PythonQt
INCLUDEPATH += $${PYTHON_QT_PATH}/include
LIBS        += -L$${PYTHON_QT_PATH}/lib
DEPENDPATH  += $${PYTHON_QT_PATH}/lib

######################################################################

## FUNCTION DEFINITION FOR COPY FUNCTION
mkpath($${PWD}/include/$${TARGET_DIR_NAME})

defineTest(copyHeaders) {

    files = $$1
    dir = $${PWD}/../../include/$${TARGET_DIR_NAME}
    win32:dir ~= s,/,\\,g

    QMAKE_POST_LINK += $$QMAKE_CHK_DIR_EXISTS $$shell_quote($$dir) $$QMAKE_MKDIR $$shell_quote($$dir) $$escape_expand(\\n\\t)

    for(file, files) {

        exists($$file) {

            win32:file ~= s,/,\\,g
            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
        }
    }

    export(QMAKE_POST_LINK)

    return(true)
}

defineTest(copyToEnvironmentPathModules) {
    environmentPath = $${GTLAB_ENVIRONMENT_PATH}/modules

    copyToEnvironmentPath($$1, $$environmentPath)
}

defineTest(copyToEnvironmentPath) {

    !isEmpty(GTLAB_ENVIRONMENT_PATH) {

        dllPath = $$1
        win32:dllPath ~= s,/,\\,g

        args = $$ARGS

        count(args, 2) {
            environmentPath = $$2
        } else {
            environmentPath = $${GTLAB_ENVIRONMENT_PATH}
        }

        win32:environmentPath ~= s,/,\\,g

        exists($$environmentPath) {

            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$dllPath) $$shell_quote($$environmentPath) $$escape_expand(\\n\\t)

            export(QMAKE_POST_LINK)

            return(true)
        } else {
            warning(GTLAB_ENVIRONMENT_PATH ($${environmentPath}) does not exist!)
        }
    }

    return(false)
}

