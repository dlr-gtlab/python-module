/* GTlab - Gas Turbine laboratory
 * Source File: icons.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 20.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/gui/icons.h"

namespace gt
{
namespace resource
{
namespace gui
{
namespace icon
{

const IconType& fileMissing()
{
    static IconType icon = gt::gui::getIcon(
        QStringLiteral(":/resourcemodule-icons/fileMissing.svg"));
    return icon;
}

}

} // namespace icon

} // namespace gui

} // namespace gt
