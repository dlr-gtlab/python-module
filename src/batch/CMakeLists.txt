# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

# python batch application

set (SOURCES
    batch-script.cpp
)

if (WIN32)
    SET (SOURCES ${SOURCES} batch-script.rc)
endif()

add_executable(GTlabPythonConsole
    ${SOURCES}
)

set_target_properties(GTlabPythonConsole PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

set_target_properties(GTlabPythonConsole PROPERTIES OUTPUT_NAME
    GTlabPythonConsole${Python3_VERSION_SUFFIX})


target_compile_definitions(GTlabPythonConsole PRIVATE
    GT_MODULE_ID="Python Batch Application"
)

target_link_libraries(GTlabPythonConsole
    PRIVATE
    Qt5::Widgets
    Qt5::Xml
    GTlab::Python
)

if (GTlab_VERSION_MAJOR GREATER_EQUAL 2)
    target_link_libraries(GTlabPythonConsole PRIVATE GTlab::Gui)
else()
    target_link_libraries(GTlabPythonConsole PRIVATE GTlab::Mdi)
endif()
