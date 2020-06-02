/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_wizardsettingsitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 29.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYWIZARDSETTINGSITEM_H
#define GTPYWIZARDSETTINGSITEM_H

#include <QHash>
#include <QVariant>

class GtpyWizardSettingsItem
{
    friend class GtpyWizardSettings;

protected:
    GtpyWizardSettingsItem();

    void registerValue(const QString& key, const QVariant& value);

    QVariant value(const QString& key);

    void remove(const QString& uuid);

private:
    QHash<QString, QVariant> m_settings;
};

#endif // GTPYWIZARDSETTINGSITEM_H
