/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_shortcut.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 01.12.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_SHORTCUTS_H
#define GTPY_SHORTCUTS_H

#include <QKeySequence>

#include <gt_application.h>

namespace gtpy
{

namespace shortcut
{

namespace cat
{

constexpr const char* PY_MODULE = "Python Module";

} // namespace cat

namespace id
{

constexpr const char* EVAL = "evaluateScript";
constexpr const char* INTERRUPT = "interruptEvaluation";

} // namespace id

void registerShortCuts();


inline QKeySequence eval()
{
    return gtApp->getShortCutSequence(id::EVAL, cat::PY_MODULE);
}

inline QKeySequence interrupt()
{
    return gtApp->getShortCutSequence(id::INTERRUPT, cat::PY_MODULE);
}

inline QString evalStr()
{
    return eval().toString();
}

inline QString interruptStr()
{
    return interrupt().toString();
}

} // shortcut

} // namespace gtpy

#endif // GTPY_SHORTCUTS_H
