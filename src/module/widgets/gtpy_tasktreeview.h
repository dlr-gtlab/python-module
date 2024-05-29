/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_tasktreeview.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_TASKTREEVIEW_H
#define GTPY_TASKTREEVIEW_H

#include "gt_pythonmodule_exports.h"

#include <QModelIndex>

#include "gt_treeview.h"

/**
 * @brief The GtpyTaskTreeView class
 */
class GT_PYTHON_EXPORT GtpyTaskTreeView : public GtTreeView
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTaskTreeView
     * @param parent Parent widget.
     */
    explicit GtpyTaskTreeView(QWidget* parent);

protected:
    /**
     * @brief Handles resize event.
     * @param event Event.
     */
    void resizeEvent(QResizeEvent* event) override;

    /**
     * @brief Handles key press event.
     * @param event Event.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief Handles mouse press event.
     * @param event Event.
     */
    void mousePressEvent(QMouseEvent* event) override;

signals:
    /// Delete Process Elements Signal
    void deleteProcessElements(const QList<QModelIndex>& indexList);
};
#endif // GTPY_TASKTREEVIEW_H
