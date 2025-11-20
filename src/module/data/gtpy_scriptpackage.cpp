/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptpackage.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_scriptpackage.h"

#include "gtpy_directory.h"

GtpyScriptPackage::GtpyScriptPackage()
{
    setObjectName("Scripts");

    auto* pyDirObj = new GtpyDirectory{};
    pyDirObj->setDefault(true);
    pyDirObj->setFlag(GtObject::UserRenamable, false);
    // pyDirObj->setFlag(GtObject::UserDeletable, false);
    appendChild(pyDirObj);
}
