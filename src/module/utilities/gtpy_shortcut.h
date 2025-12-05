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

namespace category
{

constexpr const char* PY_MODULE = "Python Module";

} // namespace category

namespace id
{

constexpr const char* EVAL = "evaluateScript";
constexpr const char* INTERRUPT = "interruptEvaluation";
constexpr const char* SAVE = "saveScript";

} // namespace id

/**
 * @brief Registers the Python Module's standard shortcuts in GTlab's shortcut
 * list.
 *
 * It should be called in the init() method of the Python Module.
 */
void registerShortCuts();


/**
 * @brief Returns the QKeySequence for the "Evaluate Script" shortcut.
 * @return The currently configured key sequence.
 */
inline QKeySequence eval()
{
    return gtApp->getShortCutSequence(id::EVAL, category::PY_MODULE);
}

/**
 * @brief Returns the QKeySequence for the "Interrupt Evaluation" shortcut.
 * @return The currently configured key sequence.
 */
inline QKeySequence interrupt()
{
    return gtApp->getShortCutSequence(id::INTERRUPT, category::PY_MODULE);
}

/**
 * @brief Returns the "Evaluate Script" shortcut as a QString.
 * @return The currently configured shortcut as a string.
 */
inline QString evalStr()
{
    return eval().toString();
}

/**
 * @brief Returns the "Interrupt Evaluation" shortcut as a QString.
 * @return The currently configured shortcut as a string.
 */
inline QString interruptStr()
{
    return interrupt().toString();
}

/**
 * @brief Formats a shortcut string for display in the GUI.
 * @param shortCutStr The shortcut string to format.
 * @return The HTML-formatted shortcut string.
 */
inline QString labelText(const QString& shortCutStr)
{
    return QStringLiteral("<font color='grey'>  (%1)</font>").arg(shortCutStr);
}

/**
 * @brief Returns the "Evaluate Script" shortcut as an HTML-formatted string.
 * @return The currently configured "Evaluate Script" shortcut formatted for
 * display.
 */
inline QString evalLabelText()
{
    return labelText(evalStr());
}

/**
 * @brief Returns the "Interrupt Evaluation" shortcut as an HTML-formatted
 * string.
 * @return The currently configured "Interrupt Evaluation" shortcut formatted
 * for display.
 */
inline QString interruptLabelText()
{
    return labelText(interruptStr());
}

} // shortcut

} // namespace gtpy

#endif // GTPY_SHORTCUTS_H
