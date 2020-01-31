/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskstylemodel.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_TASKSTYLEMODEL_H
#define GTPY_TASKSTYLEMODEL_H

#include <QModelIndex>
#include <QIdentityProxyModel>

#include "gtpy_task.h"

class GtpyTaskStyleModel: public QIdentityProxyModel
{
    Q_OBJECT

public:
    GtpyTaskStyleModel(QObject* parent = Q_NULLPTR);

    /**
     * @brief data
     * @param index
     * @param role
     * @return
     */
    virtual QVariant data(const QModelIndex& index,
                          int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /**
     * @brief Sets the rootTask.
     * @param rootTask
     */
    void setRootTask(GtpyTask* rootTask);

protected:
    /**
     * @brief Returns the item flags for the given index.
     * @param Model index
     * @return Item Flags
     */
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief canDropMimeData
     * @param data
     * @param action
     * @param row
     * @param column
     * @param parent
     * @return
     */
    bool canDropMimeData(const QMimeData* data, Qt::DropAction action,
                         int row, int column,
                         const QModelIndex& parent) const Q_DECL_OVERRIDE;

    bool moveRows(const QModelIndex& sourceParent, int sourceRow,
                  int count, const QModelIndex& destinationParent,
                  int destinationChild) Q_DECL_OVERRIDE;

    /**
     * @brief dropMimeData
     * @param data
     * @param action
     * @param row
     * @param column
     * @param parent
     * @return
     */
    bool dropMimeData(const QMimeData* data, Qt::DropAction action,
                      int row, int column,
                      const QModelIndex& parent) Q_DECL_OVERRIDE;

private:
    /// Root task
    QPointer<GtpyTask> m_rootTask;
};

#endif // GTPY_TASKSTYLEMODEL_H
