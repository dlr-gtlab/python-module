# SPDX-FileCopyrightText: 2026 German Aerospace Center (DLR)
# SPDX-License-Identifier: Apache-2.0

function(gtlab_set_python_name target version_suffix)
    if (NOT TARGET "${target}")
        message(FATAL_ERROR "gtlab_set_python_name: Unknown target '${target}'")
    endif()

    get_target_property(_output_name "${target}" OUTPUT_NAME)
    if (NOT _output_name OR _output_name MATCHES "-NOTFOUND$")
        set(_output_name "${target}")
    endif()

    if (_output_name MATCHES "^(.*)(\\.gtm\\..*)$")
        set(_output_name
            "${CMAKE_MATCH_1}${version_suffix}${CMAKE_MATCH_2}")
    else()
        string(APPEND _output_name "${version_suffix}")
    endif()

    set_target_properties("${target}" PROPERTIES OUTPUT_NAME "${_output_name}")
endfunction()
