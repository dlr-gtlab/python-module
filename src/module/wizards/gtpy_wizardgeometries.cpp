/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_wizardsettings.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 27.05.2020
 * Author: Marvin Noethen (AT-TW)
 */

#include <QSettings>
#include <QRect>

#include "gt_application.h"
#include "gt_processcomponent.h"
#include "gt_processdata.h"

#include "gtpy_wizardgeometries.h"

GtpyWizardGeometries::GtpyWizardGeometries(QObject* parent) : QObject(parent)
{

}

GtpyWizardGeometries*
GtpyWizardGeometries::instance()
{
    static GtpyWizardGeometries* retval = Q_NULLPTR;

    if (retval == Q_NULLPTR)
    {
        retval = new GtpyWizardGeometries(gtApp);
    }

    return retval;
}

void
GtpyWizardGeometries::registerGeometry(const QString& uuid, const QRect& pos)
{
    m_lastGeometry.registerValue(uuid, pos);
}

QRect
GtpyWizardGeometries::lastGeometry(const QString& uuid)
{
    return m_lastGeometry.value(uuid).toRect();
}

void
GtpyWizardGeometries::registerCursorPos(const QString& uuid, int pos)
{
    m_lastCursorPos.registerValue(uuid, pos);
}

int
GtpyWizardGeometries::lastCursorPos(const QString& uuid)
{
    return m_lastCursorPos.value(uuid).toInt();
}

void
GtpyWizardGeometries::registerVSliderPos(const QString& uuid, int pos)
{
    m_lastVSlidPos.registerValue(uuid, pos);
}

int
GtpyWizardGeometries::lastVSliderPos(const QString& uuid)
{
    return m_lastVSlidPos.value(uuid).toInt();
}

void
GtpyWizardGeometries::remove(const QString& uuid)
{
    m_lastGeometry.remove(uuid);
    m_lastCursorPos.remove(uuid);
    m_lastVSlidPos.remove(uuid);
}

void
GtpyWizardGeometries::processElementDeleted(const QString& uuid)
{
    remove(uuid);
}
