/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotconfigdialog.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (AT-TW)
 */

#ifndef GTPY_PYTHONPLOTCONFIGDIALOG_H
#define GTPY_PYTHONPLOTCONFIGDIALOG_H

#include <QDialog>

class GtpyScriptEditor;

/**
 * @brief The GtpyPythonPlotConfigDialog class
 */
class GtpyPythonPlotConfigDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Parent widget
     */
    explicit GtpyPythonPlotConfigDialog(QWidget* parent = Q_NULLPTR);

    /**
     * @brief Returns user defined script.
     * @return User defined script.
     */
    QString pythonScript();

    /**
     * @brief Sets given script.
     * @param str Initialization script.
     */
    void setScript(const QString& str);

private:
    /// Python Editor
    GtpyScriptEditor* m_editor;

};

#endif // GTPY_PYTHONPLOTCONFIGDIALOG_H
