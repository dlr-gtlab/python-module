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
unix: INCLUDEPATH += $${PYTHON_PATH}/include/python3.7m

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

    win32 {

        QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote(*.h) $$shell_quote($$dir) $$escape_expand(\\n\\t)

        dirNames =

        for(file, files) {

            exists($$file) {

                dirName = $$dirname(file)

                !isEmpty(dirName) {

                    !contains(dirNames, $$dirName) {

                        dirNames += $$dirName
                        sourceDir = $${PWD}/$${dirName}/*.h

                        win32:sourceDir ~= s,/,\\,g

                        exists($${sourceDir}) {

                            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($${sourceDir}) $$shell_quote($$dir) $$escape_expand(\\n\\t)
                        }
                    }
                }
            }
        }

    }
    unix {
        QMAKE_POST_LINK += find . -name $$shell_quote(*.h) -exec cp $$shell_quote({}) $$shell_quote($$dir) \; $$escape_expand(\\n\\t)
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

            win32: QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$dllPath) $$shell_quote($$environmentPath) $$escape_expand(\\n\\t)
            unix:  QMAKE_POST_LINK += find $$LIB_BUILD_DEST -name $$shell_quote(*.so*) -exec cp $$shell_quote({}) $$shell_quote($$environmentPath) \; $$escape_expand(\\n\\t)

            export(QMAKE_POST_LINK)

            return(true)
        } else {

            warning(GTLAB_ENVIRONMENT_PATH ($${environmentPath}) does not exist!)
        }
    }

    return(false)
}
