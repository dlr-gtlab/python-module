#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

BUILD_DEST     = ../../$${LIB_BUILD_DEST}
MOC_BUILD_DEST = ../../build

CONFIG(debug, debug|release){
    TARGET = GTlabPython$${PY_VERSION}-d
} else {
    TARGET = GTlabPython$${PY_VERSION}
}

QT += core widgets xml svg
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++11

DEFINES += GT_PYTHON_DLL

CONFIG(debug, debug|release){
    DESTDIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/rcc
    UI_DIR = $${MOC_BUILD_DEST}/debug-$${TARGET_DIR_NAME}/ui
} else {
    DESTDIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/rcc
    UI_DIR = $${MOC_BUILD_DEST}/release-$${TARGET_DIR_NAME}/ui
}

INCLUDEPATH += .\
    calculators \
    tasks \
    post \
    utilities \
    utilities/pythonextensions \
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
    utilities/gtpy_globals.h \
    utilities/pythonextensions/gtpy_calculatorsmodule.h \
    utilities/pythonextensions/gtpy_loggingmodule.h \
    utilities/pythonextensions/gtpy_projectpathfunction.h \
    wizards/gtpy_abstractscriptingwizardpage.h \
    widgets/gtpy_scripteditor.h \
    widgets/gtpy_completer.h \
    models/gtpy_completermodel.h \
    wizards/gtpy_wizardsettings.h \
    wizards/gtpy_wizardsettingsitem.h \
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
    gt_pythonmodule_exports.h \
    utilities/gtpy_codegenerator.h \
    utilities/gtpy_scriptrunnable.h \
    utilities/gtpy_gilscope.h \
    utilities/gtpy_interruptrunnable.h \
    utilities/gtpy_processdatadistributor.h \
    utilities/pythonextensions/gtpy_stdout.h \
    utilities/pythonextensions/gtpy_extendedwrapper.h \
    utilities/pythonextensions/gtpy_createhelperfunction.h \
    utilities/pythonextensions/gtpy_importfunction.h \
    utilities/pythonextensions/gtpy_propertysetter.h

SOURCES += \
    gt_python.cpp \
    calculators/gtpy_scriptcalculator.cpp \
    post/gtpy_pythonplotconfigdialog.cpp \
    post/gtpy_pythonplotdata.cpp \
    post/gtpy_pythonplotitem.cpp \
    post/gtpy_pythonplotwidget.cpp \
    post/gtpy_pythonsvgwidget.cpp \
    utilities/gtpy_globals.cpp \
    utilities/pythonextensions/gtpy_calculatorsmodule.cpp \
    utilities/pythonextensions/gtpy_loggingmodule.cpp \
    utilities/pythonextensions/gtpy_projectpathfunction.cpp \
    wizards/gtpy_abstractscriptingwizardpage.cpp \
    widgets/gtpy_scripteditor.cpp \
    widgets/gtpy_completer.cpp \
    models/gtpy_completermodel.cpp \
    wizards/gtpy_wizardsettings.cpp \
    wizards/gtpy_wizardsettingsitem.cpp \
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
    utilities/gtpy_calculatorfactory.cpp \
    utilities/gtpy_codegenerator.cpp \
    utilities/gtpy_scriptrunnable.cpp \
    utilities/gtpy_gilscope.cpp \
    utilities/gtpy_interruptrunnable.cpp \
    utilities/gtpy_processdatadistributor.cpp \
    utilities/pythonextensions/gtpy_stdout.cpp \
    utilities/pythonextensions/gtpy_extendedwrapper.cpp \
    utilities/pythonextensions/gtpy_createhelperfunction.cpp \
    utilities/pythonextensions/gtpy_importfunction.cpp \
    utilities/pythonextensions/gtpy_propertysetter.cpp



CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d -lGTlabDatamodel-d -lGTlabCalculators-d
    LIBS += -lGTlabCore-d -lGTlabMdi-d

    # GTLAB MODULES

    # THIRD PARTY
    equals(PY_VERSION, 373) {
        win32: LIBS += -lPythonQt-Qt5-Python37_d3
        unix: LIBS += -lPythonQt-Qt5-Python3.7
    } else {
        win32: LIBS += -lPythonQt-Qt5-Python27_d3
    }

} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging -lGTlabDatamodel -lGTlabCalculators
    LIBS += -lGTlabCore -lGTlabMdi

    # GTLAB MODULES

    # THIRD PARTY
    win32: LIBS += -lPythonQt-Qt5-Python$${PY_VERSION}
    unix: LIBS += -lPythonQt-Qt5-Python3.7
}

######################################################################

copyHeaders($$HEADERS)
copyToEnvironmentPathModules()
######################################################################
