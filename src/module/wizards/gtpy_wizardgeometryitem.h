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

/**
 * @brief The GtpyWizardSettingsItem class
 */
class GtpyWizardGeometryItem
{
    friend class GtpyWizardGeometries;

protected:
    /**
     * @brief GtpyWizardSettingsItem
     */
    GtpyWizardGeometryItem();

    /**
     * @brief Register a key value pair to the settings.
     * @param key Identifies the value.
     * @param value Setting value.
     */
    void registerValue(const QString& key, const QVariant& value);

    /**
     * @brief Returns the value for the given key.
     * @param key Identifies the value to return.
     * @return The value for the given key.
     */
    QVariant value(const QString& key);

    /**
     * @brief Removes the value with the given key.
     * @param key Identifies the value to remove.
     */
    void remove(const QString& key);

private:
    ///Settings
    QHash<QString, QVariant> m_settings;
};

#endif // GTPYWIZARDSETTINGSITEM_H
