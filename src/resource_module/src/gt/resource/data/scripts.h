/* GTlab - Gas Turbine laboratory
 * Source File: scripts.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */
#ifndef SCRIPTS_H
#define SCRIPTS_H

#include <gt_object.h>

namespace gt
{

namespace resource
{

namespace data
{

class Scripts : public GtObject
{
    Q_OBJECT

public:

    Q_INVOKABLE Scripts();
};

} // namespace data

} // namespace resource

} // namespace gt


#endif // SCRIPTS_H
