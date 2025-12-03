/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#ifndef PACKAGE_H
#define PACKAGE_H

#include <gt_package.h>

namespace gt
{

namespace resource
{

namespace data
{

class Package : public GtPackage
{
    Q_OBJECT

public:

    Q_INVOKABLE Package();
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // PACKAGE_H
