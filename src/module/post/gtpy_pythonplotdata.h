/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotdata.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (AT-TW)
 */

#ifndef GTPY_PYTHONPLOTDATA_H
#define GTPY_PYTHONPLOTDATA_H

#include "gt_object.h"

/**
 * @brief The GtpyPythonPlotData class
 */
class GtpyPythonPlotData : public GtObject
{
    Q_OBJECT

    Q_PROPERTY(QString script MEMBER m_script)

public:
    /**
     * @brief Constructor.
     */
    Q_INVOKABLE GtpyPythonPlotData();

    /**
     * @brief Returns saved script.
     * @return
     */
    QString script() const;

    /**
     * @brief Sets given script.
     * @param script Script string.
     */
    void setScript(const QString& script);

private:
    /// python script
    QString m_script;

};

#endif // GTPY_PYTHONPLOTDATA_H
