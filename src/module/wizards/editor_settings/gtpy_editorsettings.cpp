/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorsettings.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_editorsettings.h"

GtpyEditorSettings::GtpyEditorSettings(QObject* parent) :
    m_replaceTabBySpaces(false),
    m_tabSize(0)
{
    setParent(parent);
}

bool
GtpyEditorSettings::replaceTabBySpace() const
{
    return m_replaceTabBySpaces;
}

void
GtpyEditorSettings::setReplaceTabBySpace(bool replaceTabBySpaces)
{
    m_replaceTabBySpaces = replaceTabBySpaces;
}

int
GtpyEditorSettings::tabSize() const
{
    return m_tabSize;
}

void
GtpyEditorSettings::setTabSize(int tabSize)
{
    m_tabSize = tabSize;
}
