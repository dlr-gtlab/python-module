/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorsettings.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYEDITORSETTINGS_H
#define GTPYEDITORSETTINGS_H

#include "gt_object.h"

/**
 * @brief The GtpyEditorSettings class
 */
class GtpyEditorSettings : public GtObject
{
    Q_OBJECT

public:
    /**
     * @brief GtpyEditorSettings
     * @param parent
     */
    explicit GtpyEditorSettings(QObject* parent = Q_NULLPTR);

    bool replaceTabbySpace() const;
    void setReplaceTabBySpace(bool replaceTabbySpace);

    int tabSize() const;
    void setTabSize(int tabSize);

private:

    bool m_replaceTabbySpace;

    int m_tabSize;
};

#endif // GTPYEDITORSETTINGS_H
