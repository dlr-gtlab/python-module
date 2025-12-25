/* GTlab - Gas Turbine laboratory
 * Source File: icons.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 20.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef ICONS_H
#define ICONS_H

#include <gt_version.h>
#include <gt_icons.h>

namespace gt
{
namespace resource
{
namespace gui
{
namespace icon
{

#if GT_VERSION >= GT_VERSION_CHECK(2 ,1, 0)
using IconType = gt::gui::Icon;
#else
using IconType = QIcon;
#endif

const IconType& fileMissing();

}

} // namespace icon

} // namespace gui

} // namespace gt

#endif // ICONS_H
