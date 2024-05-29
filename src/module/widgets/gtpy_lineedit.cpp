/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_lineedit.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 24.10.2023
 * Author: Marvin Noethen (AT-TW)
 */

#include "gtpy_lineedit.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

GtpyLineEdit::GtpyLineEdit(QWidget* parent) : GtLineEdit(parent)
{

}

#else

GtpyLineEdit::GtpyLineEdit(QWidget* parent) : QLineEdit(parent)
{

}

void
GtpyLineEdit::focusInEvent(QFocusEvent* event)
{
    emit focusIn();

    QLineEdit::focusInEvent(event);
}

void
GtpyLineEdit::focusOutEvent(QFocusEvent* event)
{
    if (text().isEmpty())
    {
        emit clearFocusOut();
    }
    else
    {
        emit focusOut();
    }

    QLineEdit::focusOutEvent(event);
}


#endif
