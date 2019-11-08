#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

TARGET = GTlabPython

QT += core widgets xml svg
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++11

DEFINES += GT_PYTHON_DLL

BUILD_DEST_TEMP = ../../build

contains(COMPILE_MODE, module|moduleTests) {
    BUILD_DEST = $${BUILD_DEST_TEMP}
}

equals(COMPILE_MODE, gtlabProject) {
    BUILD_DEST = $${GTLAB_PROJECT}/build/modules
    BUILD_DEST_TEMP = $${BUILD_DEST}
}

equals(COMPILE_MODE, gtlab) {
    BUILD_DEST = $${GTLAB_PATH}/bin/modules
}

CONFIG(debug, debug|release){
    DESTDIR = $${BUILD_DEST_TEMP}/debug-python
    OBJECTS_DIR = $${BUILD_DEST_TEMP}/debug-python/obj
    MOC_DIR = $${BUILD_DEST_TEMP}/debug-python/moc
    RCC_DIR = $${BUILD_DEST_TEMP}/debug-python/rcc
    UI_DIR = $${BUILD_DEST_TEMP}/debug-python/ui
} else {
    DESTDIR = $${BUILD_DEST_TEMP}/release-python
    OBJECTS_DIR = $${BUILD_DEST_TEMP}/release-python/obj
    MOC_DIR = $${BUILD_DEST_TEMP}/release-python/moc
    RCC_DIR = $${BUILD_DEST_TEMP}/release-python/rcc
    UI_DIR = $${BUILD_DEST_TEMP}/release-python/ui
}

INCLUDEPATH += .\
    calculators \
    tasks \
    post \
    utilities \
    wizards \
    wizards/script_calculator \
    wizrads/python_task \
    models \
    widgets

DESTDIR = $${BUILD_DEST}

HEADERS += \
    gt_python.h \
    calculators/gtpy_scriptcalculator.h \
    post/gtpy_pythonplotconfigdialog.h \
    post/gtpy_pythonplotdata.h \
    post/gtpy_pythonplotitem.h \
    post/gtpy_pythonplotwidget.h \
    post/gtpy_pythonsvgwidget.h \
    wizards/gtpy_abstractscriptingwizardpage.h \
    widgets/gtpy_scripteditor.h \
    widgets/gtpy_completer.h \
    models/gtpy_completermodel.h \
    wizards/script_calculator/gtpy_scriptcalculatorwizardpage.h \
    widgets/gtpy_console.h \
    tasks/gtpy_task.h \
    models/gtpy_taskitemmodel.h \
    models/gtpy_taskstylemodel.h \
    widgets/gtpy_taskdelegate.h \
    widgets/gtpy_tasktreeview.h \
    wizards/python_task/gtpy_taskwizardpage.h \
    utilities/gtpy_decorator.h \
    utilities/gtpy_contextmanager.h \
    utilities/gtpy_calculatorfactory.h \
    gt_pythonmodule_exports.h

SOURCES += \
    gt_python.cpp \
    calculators/gtpy_scriptcalculator.cpp \
    post/gtpy_pythonplotconfigdialog.cpp \
    post/gtpy_pythonplotdata.cpp \
    post/gtpy_pythonplotitem.cpp \
    post/gtpy_pythonplotwidget.cpp \
    post/gtpy_pythonsvgwidget.cpp \
    wizards/gtpy_abstractscriptingwizardpage.cpp \
    widgets/gtpy_scripteditor.cpp \
    widgets/gtpy_completer.cpp \
    models/gtpy_completermodel.cpp \
    wizards/script_calculator/gtpy_scriptcalculatorwizardpage.cpp \
    widgets/gtpy_console.cpp \
    tasks/gtpy_task.cpp \
    models/gtpy_taskitemmodel.cpp \
    models/gtpy_taskstylemodel.cpp \
    widgets/gtpy_taskdelegate.cpp \
    widgets/gtpy_tasktreeview.cpp \
    wizards/python_task/gtpy_taskwizardpage.cpp \
    utilities/gtpy_decorator.cpp \
    utilities/gtpy_contextmanager.cpp \
    utilities/gtpy_calculatorfactory.cpp

LIBS += -lGTlabLogging -lGTlabNumerics -lGTlabDatamodel -lGTlabCalculators -lGTlabMdi -lGTlabPhysics -lGTlabCore -lPythonQt-Qt5-Python373

######################################################################
