/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_wizardsettings.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 27.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QSettings>
#include <QRect>

#include "gt_application.h"
#include "gt_processcomponent.h"
#include "gt_processdata.h"

#include "gtpy_wizardsettings.h"

GtpyWizardSettings::GtpyWizardSettings(QObject* parent) : QObject(parent)
{

}

GtpyWizardSettings*
GtpyWizardSettings::instance()
{
    static GtpyWizardSettings* retval = Q_NULLPTR;

    if (retval == Q_NULLPTR)
    {
        retval = new GtpyWizardSettings(gtApp);
    }

    return retval;
}

void
GtpyWizardSettings::registerGeometry(const QString& uuid, const QRect& pos)
{
    m_lastGeometry.registerValue(uuid, pos);
}

QRect
GtpyWizardSettings::lastGeometry(const QString& uuid)
{
    return m_lastGeometry.value(uuid).toRect();
}

void
GtpyWizardSettings::registerCursorPos(const QString& uuid, int pos)
{
    m_lastCursorPos.registerValue(uuid, pos);
}

int
GtpyWizardSettings::lastCursorPos(const QString& uuid)
{
    return m_lastCursorPos.value(uuid).toInt();
}

void
GtpyWizardSettings::registerVSliderPos(const QString& uuid, int pos)
{
    m_lastVSlidPos.registerValue(uuid, pos);
}

int
GtpyWizardSettings::lastVSliderPos(const QString& uuid)
{
    return m_lastVSlidPos.value(uuid).toInt();
}

void
GtpyWizardSettings::remove(const QString& uuid)
{
    m_lastGeometry.remove(uuid);
    m_lastCursorPos.remove(uuid);
    m_lastVSlidPos.remove(uuid);
}

void
GtpyWizardSettings::processElementDestroyed(GtProcessComponent* comp)
{
    if (!comp)
    {
        return;
    }

    GtProcessData* parent = comp->findParent<GtProcessData*>();

    if (!parent)
    {
        return;
    }

    remove(comp->uuid());
}
