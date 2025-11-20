/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_directory.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_directory.h"

#include "gtpy_globals.h"

GtpyDirectory::GtpyDirectory() :
    gt::resource::data::ProjectDirectory(gtpy::constants::PROJ_PY_SCRIPTS_DIR)
{    
    setObjectName("Python");

    setIsOpen();
    hideFileFiltersProperty();

    setFileFilters({"*.py"});
}
