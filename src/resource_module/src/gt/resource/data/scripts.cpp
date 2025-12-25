/* GTlab - Gas Turbine laboratory
 * Source File: scripts.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 28.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "scripts.h"

#include <gt_logging.h>

#include "gt/resource/data/extension.h"

namespace gt
{

namespace resource
{

namespace data
{

Scripts::Scripts()
{
    setObjectName("Scripts");
    setUserHidden(true);

    gt::resource::data::extension::appendDefaultChildren(this);

    connect(this, &Scripts::childAppended, this, [this](GtObject*, GtObject*){
        setUserHidden(false);
    });
}

} // namespace data

} // namespace resource

} // namespace gt
