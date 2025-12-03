/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_shortcut.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 01.12.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_shortcut.h"

namespace gtpy
{

namespace shortcut
{

void registerShortCuts()
{
    auto registerShortCut = [](auto id, QKeySequence shortcut) {
#if GT_VERSION >= 0x020100
        gtApp->addShortCut(id, cat::PY_MODULE, shortcut, true);
#else
        gtApp->extendShortCuts({id, cat::PY_MODULE, shortcut, true});
#endif
    };

    registerShortCut(id::EVAL, {Qt::CTRL + Qt::Key_E});
    registerShortCut(id::INTERRUPT, {Qt::CTRL + Qt::Key_I});
}

} // shortcut

} // namespace gtpy
