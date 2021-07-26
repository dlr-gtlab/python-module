/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_wizardsettingsitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 29.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_wizardgeometryitem.h"

GtpyWizardGeometryItem::GtpyWizardGeometryItem()
{

}

void
GtpyWizardGeometryItem::registerValue(const QString& key, const QVariant& value)
{
    m_settings.insert(key, value);
}

QVariant
GtpyWizardGeometryItem::value(const QString& key)
{
    return m_settings.value(key, QVariant());
}

void
GtpyWizardGeometryItem::remove(const QString& key)
{
    m_settings.remove(key);
}
