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

#include <gt/resource/url.h>
#include <gt/resource/data/helper.h>

#include "gtpy_file.h"
#include "gtpy_globals.h"

GtpyDirectory::GtpyDirectory() :
    gt::resource::data::ProjectDirectory(gtpy::constants::PROJ_PY_SCRIPTS_DIR)
{    
    setObjectName("Python");
    setFlag(GtObject::UserRenamable, false);

    setIsOpen();
    setFileFilters({"*.py"});

    hideFileFiltersProperty();
    hideDeleteFilteredOutProperty();
}

std::unique_ptr<gt::resource::data::File>
GtpyDirectory::createFileResource(const QFileInfo& file)
{
    return std::make_unique<GtpyFile>(gt::resource::url::toProjFileUrl(
        gt::resource::data::helper::findProject(uuid()), file.absoluteFilePath()));
}
