/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_code.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 14.11.2025
 * Author: Jens Schmeink (DLR AT-TWK)
 */

#include "gtpy_moduleupgrader.h"

#include "gt_logging.h"

bool
gtpy::module_upgrader::to_2_0_0::run(QDomElement& root,
                                     const QString& targetPath)
{
    gtDebug() << "Do upgrade for file" << targetPath;
}
