/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotconfigdialog.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
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
