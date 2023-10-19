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
    collection \
    collection/browser \
    collection/browser/items \
    collection/itemwidget \
    collection/localwidget \
    collection/localwidget/items \
    extensions \
    models \
    post \
    processcomponents \
    utilities \
    utilities/pythonextensions \
    widgets \
    wizards \
    wizards/editor_settings \
    wizards/editor_settings/pages \
    wizards/script_calculator \
    wizrads/python_task


DESTDIR = $${BUILD_DEST}

HEADERS += \
    collection/browser/gtpy_collectionbrowser.h \
    collection/browser/gtpy_collectionbrowsermodel.h \
    collection/browser/gtpy_collectionbrowsersortmodel.h \
    collection/browser/items/gtpy_abstractbrowseritem.h \
    collection/browser/items/gtpy_browseritem.h \
    collection/browser/items/gtpy_collapsiblebrowseritem.h \
    collection/browser/items/gtpy_rootbrowseritem.h \
    collection/gtpy_collectionwidget.h \
    collection/gtpy_scriptcollectionsettings.h \
    collection/itemwidget/gtpy_collectionitemwidget.h \
    collection/localwidget/gtpy_collectionlocalmodel.h \
    collection/localwidget/gtpy_collectionlocalwidget.h \
    collection/localwidget/items/gtpy_abstractlocalitem.h \
    collection/localwidget/items/gtpy_collapsiblelocalitem.h \
    collection/localwidget/items/gtpy_localitem.h \
    extensions/gtpy_pythonfunctions.h \
    gt_compat.h \
    gt_python.h \
    gt_pythonmodule_exports.h \
    gtpy_icons_compat.h \
    gtpy_stylesheet_compat.h \
    models/gtpy_completermodel.h \
    models/gtpy_objectmodel.h \
    models/gtpy_taskstylemodel.h \
    post/gtpy_pythonplotconfigdialog.h \
    post/gtpy_pythonplotdata.h \
    post/gtpy_pythonplotitem.h \
    post/gtpy_pythonplotwidget.h \
    post/gtpy_pythonsvgwidget.h \
    processcomponents/gtpy_abstractscriptcomponent.h \
    processcomponents/gtpy_scriptcalculator.h \
    processcomponents/gtpy_task.h \
    utilities/gtpy_calculatorfactory.h \
    utilities/gtpy_codegenerator.h \
    utilities/gtpy_constants.h \
    utilities/gtpy_contextmanager.h \
    utilities/gtpy_decorator.h \
    utilities/gtpy_gilscope.h \
    utilities/gtpy_globals.h \
    utilities/gtpy_interruptrunnable.h \
    utilities/gtpy_matplotlib.h \
    utilities/gtpy_packageiteration.h \
    utilities/gtpy_processdatadistributor.h \
    utilities/gtpy_regexp.h \
    utilities/gtpy_scriptrunnable.h \
    utilities/gtpy_tempdir.h \
    utilities/gtpy_transfer.h \
    utilities/pythonextensions/gtpy_createhelperfunction.h \
    utilities/pythonextensions/gtpy_extendedwrapper.h \
    utilities/pythonextensions/gtpy_importfunction.h \
    utilities/pythonextensions/gtpy_loggingmodule.h \
    utilities/pythonextensions/gtpy_propertysetter.h \
    utilities/pythonextensions/gtpy_stdout.h \
    widgets/gtpy_completer.h \
    widgets/gtpy_console.h \
    widgets/gtpy_scripteditor.h \
    widgets/gtpy_scriptview.h \
    widgets/gtpy_taskdelegate.h \
    widgets/gtpy_tasktreeview.h \
    wizards/editor_settings/gtpy_editorsettings.h \
    wizards/editor_settings/gtpy_editorsettingsdialog.h \
    wizards/editor_settings/pages/gtpy_abstractsettingspage.h \
    wizards/editor_settings/pages/gtpy_generalpage.h \
    wizards/gtpy_abstractscriptingwizardpage.h \
    wizards/gtpy_wizardgeometries.h \
    wizards/gtpy_wizardgeometryitem.h \
    wizards/python_task/gtpy_taskwizardpage.h \
    wizards/script_calculator/gtpy_scriptcalculatorwizardpage.h


SOURCES += \
    collection/browser/gtpy_collectionbrowser.cpp \
    collection/browser/gtpy_collectionbrowsermodel.cpp \
    collection/browser/gtpy_collectionbrowsersortmodel.cpp \
    collection/browser/items/gtpy_abstractbrowseritem.cpp \
    collection/browser/items/gtpy_browseritem.cpp \
    collection/browser/items/gtpy_collapsiblebrowseritem.cpp \
    collection/browser/items/gtpy_rootbrowseritem.cpp \
    collection/gtpy_collectionwidget.cpp \
    collection/gtpy_scriptcollectionsettings.cpp \
    collection/itemwidget/gtpy_collectionitemwidget.cpp \
    collection/localwidget/gtpy_collectionlocalmodel.cpp \
    collection/localwidget/gtpy_collectionlocalwidget.cpp \
    collection/localwidget/items/gtpy_abstractlocalitem.cpp \
    collection/localwidget/items/gtpy_collapsiblelocalitem.cpp \
    collection/localwidget/items/gtpy_localitem.cpp \
    extensions/gtpy_pythonfunctions.cpp \
    gt_python.cpp \
    models/gtpy_completermodel.cpp \
    models/gtpy_objectmodel.cpp \
    models/gtpy_taskstylemodel.cpp \
    post/gtpy_pythonplotconfigdialog.cpp \
    post/gtpy_pythonplotdata.cpp \
    post/gtpy_pythonplotitem.cpp \
    post/gtpy_pythonplotwidget.cpp \
    post/gtpy_pythonsvgwidget.cpp \
    processcomponents/gtpy_abstractscriptcomponent.cpp \
    processcomponents/gtpy_scriptcalculator.cpp \
    processcomponents/gtpy_task.cpp \
    utilities/gtpy_calculatorfactory.cpp \
    utilities/gtpy_codegenerator.cpp \
    utilities/gtpy_contextmanager.cpp \
    utilities/gtpy_decorator.cpp \
    utilities/gtpy_gilscope.cpp \
    utilities/gtpy_globals.cpp \
    utilities/gtpy_interruptrunnable.cpp \
    utilities/gtpy_processdatadistributor.cpp \
    utilities/gtpy_regexp.cpp \
    utilities/gtpy_scriptrunnable.cpp \
    utilities/gtpy_tempdir.cpp \
    utilities/gtpy_transfer.cpp \
    utilities/pythonextensions/gtpy_calculatorsmodule.cpp \
    utilities/pythonextensions/gtpy_createhelperfunction.cpp \
    utilities/pythonextensions/gtpy_extendedwrapper.cpp \
    utilities/pythonextensions/gtpy_importfunction.cpp \
    utilities/pythonextensions/gtpy_loggingmodule.cpp \
    utilities/pythonextensions/gtpy_propertysetter.cpp \
    utilities/pythonextensions/gtpy_stdout.cpp \
    widgets/gtpy_completer.cpp \
    widgets/gtpy_console.cpp \
    widgets/gtpy_scripteditor.cpp \
    widgets/gtpy_scriptview.cpp \
    widgets/gtpy_taskdelegate.cpp \
    widgets/gtpy_tasktreeview.cpp \
    wizards/editor_settings/gtpy_editorsettings.cpp \
    wizards/editor_settings/gtpy_editorsettingsdialog.cpp \
    wizards/editor_settings/pages/gtpy_abstractsettingspage.cpp \
    wizards/editor_settings/pages/gtpy_generalpage.cpp \
    wizards/gtpy_abstractscriptingwizardpage.cpp \
    wizards/gtpy_wizardgeometries.cpp \
    wizards/gtpy_wizardgeometryitem.cpp \
    wizards/python_task/gtpy_taskwizardpage.cpp \
    wizards/script_calculator/gtpy_scriptcalculatorwizardpage.cpp

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
    greaterThan(MAJOR_VERSION, 1) {
        win32: LIBS += -lPythonQt-Qt5-Python$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}_d
    } else {
        win32: LIBS += -lPythonQt-Qt5-Python$${PY_MAJOR_VERSION}$${PY_MINOR_VERSION}_d
    }

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
    greaterThan(MAJOR_VERSION, 1) {
        win32: LIBS += -lPythonQt-Qt5-Python$${PY_MAJOR_VERSION}.$${PY_MINOR_VERSION}
    } else {
        win32: LIBS += -lPythonQt-Qt5-Python$${PY_MAJOR_VERSION}$${PY_MINOR_VERSION}
    }

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
