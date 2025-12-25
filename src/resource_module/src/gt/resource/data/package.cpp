/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#include "gt/resource/data/package.h"

#include <gt_logging.h>

#include "gt/resource/data/scripts.h"
#include "gt/resource/data/extension.h"

namespace gt
{

namespace resource
{

namespace data
{

Package::Package()
{
    setObjectName("Resources");

    auto scripts = std::make_unique<Scripts>();
    scripts->setDefault(true);
    if (appendChild(scripts.get())) scripts.release();

    gt::resource::data::extension::appendDefaultChildren(this);
}

} // namespace data

} // namespace resource

} // namespace gt
