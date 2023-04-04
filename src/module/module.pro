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

GT_MODULE_ID="Python Module (Python $${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION})"

CONFIG(debug, debug|release){
    TARGET = GTlabPython$${PY_VERSION}-d
} else {
    TARGET = GTlabPython$${PY_VERSION}
}

QT += core widgets xml svg
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++14

DEFINES += GT_PYTHON_DLL
DEFINES += GT_LOG_USE_QT_BINDINGS

isEmpty(GT_MODULE_ID) {
   error("GT_MODULE_ID undefined. Please define variable GT_MODULE_ID=\"My Module ID\" in project file.")
}

DEFINES += GT_MODULE_ID='"\\\"$${GT_MODULE_ID}\\\""'
DEFINES += PY_MAJOR_VERSION=$${PY_MAJOR_VERSION}
DEFINES += PY_MINOR_VERSION=$${PY_MINOR_VERSION}

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
    collection \
    collection/browser \
    collection/browser/items \
    collection/itemwidget \
    collection/localwidget \
    collection/localwidget/items \
    tasks \
    post \
    extensions \
    utilities \
    utilities/pythonextensions \
    wizards \
    wizards/script_calculator \
    wizrads/python_task \
    wizards/editor_settings \
    wizards/editor_settings/pages \
    models \
    widgets

DESTDIR = $${BUILD_DEST}

HEADERS += \
    collection/browser/gtpy_collectionbrowsersortmodel.h \
    collection/browser/items/gtpy_abstractbrowseritem.h \
    collection/browser/items/gtpy_browseritem.h \
    collection/browser/items/gtpy_collapsiblebrowseritem.h \
    collection/browser/gtpy_collectionbrowser.h \
    collection/browser/gtpy_collectionbrowsermodel.h \
    collection/browser/items/gtpy_rootbrowseritem.h \
    collection/itemwidget/gtpy_collectionitemwidget.h \
    collection/localwidget/gtpy_collectionlocalmodel.h \
    collection/localwidget/gtpy_collectionlocalwidget.h \
    collection/localwidget/items/gtpy_abstractlocalitem.h \
    collection/localwidget/items/gtpy_collapsiblelocalitem.h \
    collection/localwidget/items/gtpy_localitem.h \
    extensions/gtpy_pythonfunctions.h \
    gt_python.h \
    gt_compat.h \
    calculators/gtpy_scriptcalculator.h \
    collection/gtpy_scriptcollectionsettings.h \
    collection/gtpy_collectionwidget.h \
    gtpy_icons_compat.h \
    gtpy_stylesheet_compat.h \
    models/gtpy_objectmodel.h \
    post/gtpy_pythonplotconfigdialog.h \
    post/gtpy_pythonplotdata.h \
    post/gtpy_pythonplotitem.h \
    post/gtpy_pythonplotwidget.h \
    post/gtpy_pythonsvgwidget.h \
    utilities/gtpy_constants.h \
    utilities/gtpy_globals.h \
    utilities/gtpy_packageiteration.h \
    utilities/gtpy_regexp.h \
    utilities/gtpy_utilities.h \
    utilities/pythonextensions/gtpy_loggingmodule.h \
    wizards/gtpy_abstractscriptingwizardpage.h \
    widgets/gtpy_scripteditor.h \
    widgets/gtpy_completer.h \
    models/gtpy_completermodel.h \
    wizards/gtpy_wizardgeometries.h \
    wizards/gtpy_wizardgeometryitem.h \
    wizards/script_calculator/gtpy_scriptcalculatorwizardpage.h \
    widgets/gtpy_console.h \
    tasks/gtpy_task.h \
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
    utilities/pythonextensions/gtpy_propertysetter.h \
    wizards/editor_settings/gtpy_editorsettings.h \
    wizards/editor_settings/gtpy_editorsettingsdialog.h \
    wizards/editor_settings/pages/gtpy_abstractsettingspage.h \
    wizards/editor_settings/pages/gtpy_generalpage.h

SOURCES += \
    collection/browser/gtpy_collectionbrowsersortmodel.cpp \
    collection/browser/items/gtpy_abstractbrowseritem.cpp \
    collection/browser/items/gtpy_browseritem.cpp \
    collection/browser/items/gtpy_collapsiblebrowseritem.cpp \
    collection/browser/gtpy_collectionbrowser.cpp \
    collection/browser/gtpy_collectionbrowsermodel.cpp \
    collection/browser/items/gtpy_rootbrowseritem.cpp \
    collection/itemwidget/gtpy_collectionitemwidget.cpp \
    collection/localwidget/gtpy_collectionlocalmodel.cpp \
    collection/localwidget/gtpy_collectionlocalwidget.cpp \
    collection/localwidget/items/gtpy_abstractlocalitem.cpp \
    collection/localwidget/items/gtpy_collapsiblelocalitem.cpp \
    collection/localwidget/items/gtpy_localitem.cpp \
    extensions/gtpy_pythonfunctions.cpp \
    gt_python.cpp \
    calculators/gtpy_scriptcalculator.cpp \
    collection/gtpy_scriptcollectionsettings.cpp \
    collection/gtpy_collectionwidget.cpp \
    models/gtpy_objectmodel.cpp \
    post/gtpy_pythonplotconfigdialog.cpp \
    post/gtpy_pythonplotdata.cpp \
    post/gtpy_pythonplotitem.cpp \
    post/gtpy_pythonplotwidget.cpp \
    post/gtpy_pythonsvgwidget.cpp \
    utilities/gtpy_globals.cpp \
    utilities/gtpy_regexp.cpp \
    utilities/gtpy_utilities.cpp \
    utilities/pythonextensions/gtpy_calculatorsmodule.cpp \
    utilities/pythonextensions/gtpy_loggingmodule.cpp \
    wizards/gtpy_abstractscriptingwizardpage.cpp \
    widgets/gtpy_scripteditor.cpp \
    widgets/gtpy_completer.cpp \
    models/gtpy_completermodel.cpp \
    wizards/gtpy_wizardgeometries.cpp \
    wizards/gtpy_wizardgeometryitem.cpp \
    wizards/script_calculator/gtpy_scriptcalculatorwizardpage.cpp \
    widgets/gtpy_console.cpp \
    tasks/gtpy_task.cpp \
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
    utilities/pythonextensions/gtpy_propertysetter.cpp \
    wizards/editor_settings/gtpy_editorsettings.cpp \
    wizards/editor_settings/gtpy_editorsettingsdialog.cpp \
    wizards/editor_settings/pages/gtpy_abstractsettingspage.cpp \
    wizards/editor_settings/pages/gtpy_generalpage.cpp



CONFIG(debug, debug|release){
    # GTLAB CORE
    LIBS += -lGTlabLogging-d

    greaterThan(MAJOR_VERSION, 1) {
        message(GTlab Version 2 or newer)
        LIBS += -lGTlabCore-d -lGTlabGui-d -lGTlabDataProcessor-d
    } else {
        message(GTlab Version older than 2)
        # GTLAB CORE
        LIBS += -lGTlabDatamodel-d -lGTlabCalculators-d
        LIBS += -lGTlabCore-d -lGTlabMdi-d -lGTlabNetwork-d
    }

    # THIRD PARTY
    win32: LIBS += -lPythonQt-Qt5-Python$${PY_VERSION}_d

    unix {
        LIBS += -lPythonQt-Qt5-Python$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}
        equals(PY_VERSION, 37) {
            DEFINES += PYTHON_LIBRARY=\\\"libpython3.7m.so.1.0\\\"
        } else {
            DEFINES += PYTHON_LIBRARY=\\\"libpython$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}d.so.1.0\\\"
        }
    }
} else {
    # GTLAB CORE
    LIBS += -lGTlabLogging

    greaterThan(MAJOR_VERSION, 1) {
        message(GTlab Version 2 or newer)
        LIBS += -lGTlabCore -lGTlabGui -lGTlabDataProcessor
    } else {
        message(GTlab Version older than 2)
        # GTLAB CORE
        LIBS += -lGTlabDatamodel -lGTlabCalculators
        LIBS += -lGTlabCore -lGTlabMdi -lGTlabNetwork
    }

    # GTLAB MODULES

    # THIRD PARTY
    win32: LIBS += -lPythonQt-Qt5-Python$${PY_VERSION}
    unix {
        LIBS += -lPythonQt-Qt5-Python$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}

        equals(PY_VERSION, 37) {

            DEFINES += PYTHON_LIBRARY=\\\"libpython3.7m.so.1.0\\\"
        } else {

            DEFINES += PYTHON_LIBRARY=\\\"libpython$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}.so.1.0\\\"
        }
   }
}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/..\''
}

######################################################################

copyHeaders($$HEADERS)
copyToEnvironmentPathModules()
######################################################################
