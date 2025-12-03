/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_code.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 14.11.2025
 * Author: Jens Schmeink (DLR AT-TWK)
 */
#ifndef GTPY_MODULEUPGRADER_H
#define GTPY_MODULEUPGRADER_H

#include <QDomElement>

namespace gtpy {

namespace module_upgrader {

namespace to_2_0_0 {
    /**
    * @brief Performs the upgrade to predign data model 3.0.0
    *
    * @return true in case of success
    */
    bool run(QDomElement& root, const QString& targetPath);
} // to_2_0_0
} // module_upgrader
} // gtpy


#endif // GTPY_MODULEUPGRADER_H
