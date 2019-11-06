/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_tasktreeview.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QScrollBar>
#include <QKeyEvent>

#include "gtpy_taskdelegate.h"

#include "gtpy_tasktreeview.h"

GtpyTaskTreeView::GtpyTaskTreeView(QWidget* parent) :
    GtTreeView(parent)
{
    setHeaderHidden(true);
    setAnimated(true);
    setAlternatingRowColors(true);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(InternalMove);
    setDefaultDropAction(Qt::MoveAction);
    setDropIndicatorShown(true);
    setExpandsOnDoubleClick(false);
    setEditTriggers(QTreeView::SelectedClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    GtpyTaskDelegate* delegate = new GtpyTaskDelegate(this);
    setItemDelegate(delegate);
}

void
GtpyTaskTreeView::resizeEvent(QResizeEvent* event)
{
    setColumnWidth(1, 20);

    int delta = 0;

    if (verticalScrollBar()->isVisible())
    {
        delta = verticalScrollBar()->width();
    }

    setColumnWidth(0, width() - 33 - delta);

    QTreeView::resizeEvent(event);
}

void
GtpyTaskTreeView::keyPressEvent(QKeyEvent* event)
{
    QList<QModelIndex> indexes = selectionModel()->selectedIndexes();

    if (event->key() == Qt::Key_Delete)
    {
        emit deleteProcessElements(indexes);
    }

    GtTreeView::keyPressEvent(event);
}

void
GtpyTaskTreeView::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);

    QModelIndex index = indexAt(event->pos());

    if (!index.isValid())
    {
        clearSelection();
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
        emit clicked(index);
    }
}
