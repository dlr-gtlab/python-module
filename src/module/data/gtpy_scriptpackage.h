/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_scriptpackage.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_SCRIPTPACKAGE_H
#define GTPY_SCRIPTPACKAGE_H

#include <gt_package.h>

class GtpyScriptPackage : public GtPackage
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyScriptPackage();
};

#endif // GTPY_SCRIPTPACKAGE_H
