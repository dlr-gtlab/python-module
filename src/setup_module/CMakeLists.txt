#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
#
######################################################################

# only for GTlab 2.x

if (GTlab_VERSION_MAJOR LESS 2)
    return()
endif()

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(HEADERS
    find_libpython.h
    gt_python_setup.h
    gtps_constants.h
    gtps_globals.h
    gtps_pythoninterpreter.h
    gtps_pythonpreferencepage.h
    gtps_systemsettings.h
)

set(SOURCES
    gt_python_setup.cpp
    gtps_globals.cpp
    gtps_pythoninterpreter.cpp
    gtps_pythonpreferencepage.cpp
    gtps_systemsettings.cpp
    resources.qrc
    gtps_pythonpreferencepage.ui
)

add_gtlab_module(GTlabPythonSetup SOURCES ${HEADERS} ${SOURCES}
    MODULE_ID "Python Setup"
)

target_compile_definitions(GTlabPythonSetup PRIVATE
    GT_PYTHON_SETUP_DLL
)

target_include_directories(GTlabPythonSetup PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
)

target_link_libraries(GTlabPythonSetup PRIVATE GTlab::Gui)
