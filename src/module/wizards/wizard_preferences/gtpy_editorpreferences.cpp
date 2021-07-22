/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorpreferences.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_editorpreferences.h"

GtpyEditorPreferences::GtpyEditorPreferences(QObject* parent)
{
    setParent(parent);
}

GtpyEditorPreferences::~GtpyEditorPreferences()
{

}

bool
GtpyEditorPreferences::replaceTabbySpace() const
{
    return m_replaceTabbySpace;
}

void
GtpyEditorPreferences::setReplaceTabBySpace(bool replaceTabbySpace)
{
    m_replaceTabbySpace = replaceTabbySpace;
}

int
GtpyEditorPreferences::tabSize() const
{
    return m_tabSize;
}

void
GtpyEditorPreferences::setTabSize(int tabSize)
{
    m_tabSize = tabSize;
}
