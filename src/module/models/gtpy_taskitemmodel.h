/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskitemmodel.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_TASKITEMMODEL_H
#define GTPY_TASKITEMMODEL_H

#include <QModelIndex>

#include "gt_objectmodel.h"

/**
 * @brief The GtpyTaskItemModel class
 */
class GtpyTaskItemModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTaskItemModel
     * @param root Root object.
     * @param parent Parent object.
     */
    GtpyTaskItemModel(GtObject* root, QObject* parent = Q_NULLPTR);

    /**
     * @brief Returns column count.
     * @param parent
     * @return Column count.
     */
    virtual int columnCount(
        const QModelIndex& /*parent*/) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns row count.
     * @param parent Parent object.
     * @return Row count.
     */
    virtual int rowCount(
        const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * @brief index Returns index of current row and column.
     * @param row Row value.
     * @param col Column value.
     * @param parent Parent object.
     * @return Index of current row and column
     */
    virtual QModelIndex index(int row, int col, const QModelIndex& parent =
                                  QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns parent index of given index.
     * @param index Index whose parent should be found.
     * @return Parent index of given index.
     */
    virtual QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns data of given index.
     * @param index Index value.
     * @param role Role of item.
     * @return Data of given index.
     */
    virtual QVariant data(const QModelIndex& index,
                          int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns object from index.
     * @param index Index of object.
     * @return Object from index.
     */
    virtual GtObject* objectFromIndex(const QModelIndex& index) const;

    /**
     * @brief Returns index from object.
     * @param obj Object.
     * @return Index of object.
     */
    virtual QModelIndex indexFromObject(GtObject* obj) const;

    /**
    * @brief Sets the role data for the item at index to value.
    * @param Model index
    * @param Value
    * @param Data role
    * @return Returns true if successful; otherwise returns false.
    */
    virtual bool setData(const QModelIndex& index,
                         const QVariant& value,
                         int role = Qt::EditRole) Q_DECL_OVERRIDE;

    /**
     * @brief Returns list of mime types.
     * @return List of mime types.
     */
    QStringList mimeTypes() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns an object that contains serialized items of data
     * corresponding to the list of indexes specified.
     * @param List of model indexes
     * @return MimeData
     */
    QMimeData* mimeData(const QModelIndexList& indexes) const Q_DECL_OVERRIDE;

    /**
    * @brief Updates view.
    */
    void updateView();

protected:
    /**
     * @brief Returns the drop actions supported by this model.
     * @return Supported drop actions
     */
    Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the actions supported by the data in this model.
     * @return Supported drag actions
     */
    Qt::DropActions supportedDragActions() const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the item flags for the given index.
     * @param Model index
     * @return Item Flags
     */
    Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

private:
    /// Root object.
    QPointer<GtObject> m_root;

    /// GtObjectModel
    GtObjectModel* m_objectModel;

signals:
    void processComponentRenamed(QString className, QString oldName,
                                 QString newName);
};

#endif // GTPY_TASKITEMMODEL_H
