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

#include "gt_pythonmodule_exports.h"

#include <QModelIndex>
#include <QIdentityProxyModel>

#include "gtpy_task.h"

/**
 * @brief The GtpyTaskStyleModel class
 */
class GT_PYTHON_EXPORT GtpyTaskStyleModel: public QIdentityProxyModel
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTaskStyleModel
     * @param parent Parent object.
     */
    explicit GtpyTaskStyleModel(QObject* parent = nullptr);

    /**
     * @brief data
     * @param index
     * @param role
     * @return
     */
    virtual QVariant data(const QModelIndex& index,
                          int role = Qt::DisplayRole) const override;

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
    Qt::ItemFlags flags(const QModelIndex& index) const override;

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
                         const QModelIndex& parent) const override;

    bool moveRows(const QModelIndex& sourceParent, int sourceRow,
                  int count, const QModelIndex& destinationParent,
                  int destinationChild) override;

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
                      const QModelIndex& parent) override;

private:
    /// Root task
    QPointer<GtpyTask> m_rootTask;
};

#endif // GTPY_TASKSTYLEMODEL_H
