/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_preferenceseditor.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.07.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYPREFERENCESEDITOR_H
#define GTPYPREFERENCESEDITOR_H

#include "gtpy_preferencespage.h"

class GtpyPreferencesEditor : public GtpyPreferencesPage
{
    Q_OBJECT

public:
    GtpyPreferencesEditor();

    void saveSettings() Q_DECL_OVERRIDE;

private:
    /**
     * @brief loadSettings
     */
    void loadSettings();
};

#endif // GTPYPREFERENCESEDITOR_H
