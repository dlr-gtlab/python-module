/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_editorsettings.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.07.2021
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPYEDITORSETTINGS_H
#define GTPYEDITORSETTINGS_H

#include "gt_pythonmodule_exports.h"

#include "gt_object.h"

/**
 * @brief The GtpyEditorSettings class
 */
class GT_PYTHON_EXPORT GtpyEditorSettings : public GtObject
{
    Q_OBJECT

public:
    /**
     * @brief GtpyEditorSettings
     * @param parent
     */
    explicit GtpyEditorSettings(QObject* parent = nullptr);

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
