#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __
#          / /_/ / / / / / /_/ / /_/ /
#         ____/ /_/ /_/\__,_/_.___/
#
######################################################################

# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

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
    gtps_pythonpreferencepage.ui
)

add_gtlab_module(GTlabPythonSetup SOURCES ${HEADERS} ${SOURCES}
    MODULE_ID "Python Setup"
)

target_compile_definitions(GTlabPythonSetup PRIVATE
    GT_PYTHON_SETUP_DLL
)

# The module ABI is independent of the Core version. Pass the actual value
# exported by Core so runtime discovery accepts only compatible modules.
set(_gtlab_python_module_abi "${GTLAB_MODULE_ABI}")
if (NOT _gtlab_python_module_abi)
    get_property(_gtlab_python_module_abi GLOBAL PROPERTY GTLAB_MODULE_ABI)
endif()

if (_gtlab_python_module_abi)
    target_compile_definitions(GTlabPythonSetup PRIVATE
        "GTLAB_MODULE_ABI=\"${_gtlab_python_module_abi}\"")
elseif (DEFINED GTLAB_CORE_VERSION AND
        GTLAB_CORE_VERSION VERSION_GREATER_EQUAL "2.1")
    message(FATAL_ERROR
        "GTlab ${GTLAB_CORE_VERSION} requires GTLAB_MODULE_ABI for Python module discovery")
endif()

target_include_directories(GTlabPythonSetup PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/.>
)

target_link_libraries(GTlabPythonSetup PRIVATE GTlab::Gui)
