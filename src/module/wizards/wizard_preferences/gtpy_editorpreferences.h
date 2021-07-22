/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorpreferences.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYEDITORPREFERENCES_H
#define GTPYEDITORPREFERENCES_H

#include "gt_object.h"

class GtpyEditorPreferences : public GtObject
{
    Q_OBJECT

public:
    GtpyEditorPreferences(QObject* parent = Q_NULLPTR);

    ~GtpyEditorPreferences();

    bool replaceTabbySpace() const;
    void setReplaceTabBySpace(bool replaceTabbySpace);

    int tabSize() const;
    void setTabSize(int tabSize);

private:

    bool m_replaceTabbySpace;

    int m_tabSize;
};

#endif // GTPYEDITORPREFERENCES_H
