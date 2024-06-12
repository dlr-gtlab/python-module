# SPDX-License-Identifier: Apache-2.0
# SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)

option(BUILD_BUNDLED_PYTHONQT "Build bundled pythonqt as part of this build (recommended)" ON)

if (BUILD_BUNDLED_PYTHONQT)
  set(PythonQt_INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_BINDIR})
  add_subdirectory(pythonqt)
endif()
