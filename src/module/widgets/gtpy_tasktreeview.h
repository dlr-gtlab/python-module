/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_tasktreeview.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_TASKTREEVIEW_H
#define GTPY_TASKTREEVIEW_H

#include <QModelIndex>

#include "gt_treeview.h"

/**
 * @brief The GtpyTaskTreeView class
 */
class GtpyTaskTreeView : public GtTreeView
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTaskTreeView
     * @param parent Parent widget.
     */
    GtpyTaskTreeView(QWidget* parent);

protected:
    /**
     * @brief Handles resize event.
     * @param event Event.
     */
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    /**
     * @brief Handles key press event.
     * @param event Event.
     */
    void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

    /**
     * @brief Handles mouse press event.
     * @param event Event.
     */
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

signals:
    /// Delete Process Elements Signal
    void deleteProcessElements(const QList<QModelIndex>& indexList);
};
#endif // GTPY_TASKTREEVIEW_H
