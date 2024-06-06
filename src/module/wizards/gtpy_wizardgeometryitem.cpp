/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_wizardsettingsitem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 29.05.2020
 * Author: Marvin Noethen (DLR AT-TWK)
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
