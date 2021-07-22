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

    /**
     * @brief Returns whether tabs are replaced by spaces.
     * @return True if tabs are replaced by spaces.
     */
    bool replaceTabBySpace() const;

    /**
     * @brief Sets whether tabs are replaced by spaces.
     * @param replaceTabBySpaces If this parameter is true tabs will be
     * replaced by spaces.
     */
    void setReplaceTabBySpace(bool replaceTabBySpaces);

    /**
     * @brief Returns the tab size.
     * @return Tab size
     */
    int tabSize() const;

    /**
     * @brief Sets the tab size to the given tabSize.
     * @param tabSize Tab size value.
     */
    void setTabSize(int tabSize);

private:
    /// Replace Tab By Spaces
    bool m_replaceTabBySpaces;

    /// Tab size
    int m_tabSize;
};

#endif // GTPYEDITORSETTINGS_H
