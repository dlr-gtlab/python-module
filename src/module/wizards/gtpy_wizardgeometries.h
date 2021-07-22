/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_wizardsettings.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 27.05.2020
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYWIZARDSETTINGS_H
#define GTPYWIZARDSETTINGS_H

#include <QObject>

#include "gtpy_wizardgeometryitem.h"

class GtProcessComponent;

/**
 * @brief The GtpyWizardGeometries class
 */
class GtpyWizardGeometries : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Returns instance of GtpyWizardGeometries based on singleton
     * pattern.
     * @return Instance of GtpyWizardGeometries.
     */
    static GtpyWizardGeometries* instance();

    /**
     * @brief Regeisters the given geometry of the wizard to the settings item.
     * @param uuid Uuid of the process component.
     * @param pos Geometry to register.
     */
    void registerGeometry(const QString& uuid, const QRect& rect);

    /**
     * @brief Returns the last geometry position of the wizard.
     * @param uuid Uuid of the process component.
     * @return The last registered geometry of the wizard.
     */
    QRect lastGeometry(const QString& uuid);

    /**
     * @brief Regeisters the given cursor position to the settings item.
     * @param uuid Uuid of the process component.
     * @param pos Cursor position to register.
     */
    void registerCursorPos(const QString& uuid, int pos);

    /**
     * @brief Returns the last registered cursor position.
     * @param uuid Uuid of the process component.
     * @return The last registered cursor position.
     */
    int lastCursorPos(const QString& uuid);

    /**
     * @brief Regeisters the given vertical slider position to the settings
     * item.
     * @param uuid Uuid of the process component.
     * @param pos Vertical slider position to register.
     */
    void registerVSliderPos(const QString& uuid, int pos);

    /**
     * @brief Returns the last registered vertical slider position.
     * @param uuid Uuid of the process component.
     * @return The last registered vertical slider position.
     */
    int lastVSliderPos(const QString& uuid);

private:
    /**
     * @brief GtpyWizardGeometries
     * @param parent
     */
    explicit GtpyWizardGeometries(QObject* parent = Q_NULLPTR);

    /**
     * @brief Removes the sttings entries identified by the given uuid.
     * @param uuid
     */
    void remove(const QString& uuid);

    ///Last geometry settings
    GtpyWizardGeometryItem m_lastGeometry;

    ///Last cursor position
    GtpyWizardGeometryItem m_lastCursorPos;

    ///Last slider position
    GtpyWizardGeometryItem m_lastVSlidPos;

private slots:
    /**
     * @brief Removes the settings for the destroyed process component.
     * @param comp Destroyed process compenent.
     */
    void processElementDeleted(const QString& uuid);
};

#endif // GTPYWIZARDSETTINGS_H
