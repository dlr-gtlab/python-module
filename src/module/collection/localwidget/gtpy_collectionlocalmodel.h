/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionlocalmodel.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONLOCALMODEL_H
#define GTPYCOLLECTIONLOCALMODEL_H

#include <QAbstractItemModel>

#include "gt_collectionitem.h"

class GtpyCollapsibleLocalItem;

/**
 * @brief The GtpyCollectionLocalModel class
 */
class GtpyCollectionLocalModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionLocalModel
     * @param parent Parent object.
     */
    explicit GtpyCollectionLocalModel(QObject* parent = Q_NULLPTR);

    /**
     * @brief ~GtpyCollectionLocalModel
     */
    virtual ~GtpyCollectionLocalModel();

    /**
     * @brief Returns the row count under the given parent.
     * @param parent Parent index.
     * @return The row count under the given parent.
     */
    int rowCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the column count for the children under the given parent.
     * @param parent Parent index.
     * @return The column count for the children under the given parent.
     */
    int columnCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the data for the item at the given index.
     * @param index Index of the item.
     * @param role Type of data.
     * @return The data for the item at the given index.
     */
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the header data.
     * @param section Column number.
     * @param orientation Orientation of the header.
     * @param role Type of data.
     * @return The header data.
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

    /**
     * @brief Appends the given list of items as children of the root item.
     * @param items List of items.
     */
    void setItems(const QList<GtCollectionItem>& items);

    /**
     * @brief Returns the index of the item in the model.
     * @param row Row of the item.
     * @param column Column of the item.
     * @param parent Parent of the item.
     * @return Index of the item in the model.
     */
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the parent index of the item with the given index.
     * @param index Index of an item of the model.
     * @return The parent index of the item with the given index.
     */
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief Retruns the collection item with the given index.
     * @param index Index of the item.
     * @return The collection item with the given index.
     */
    GtCollectionItem itemFromIndex(const QModelIndex& index);

    /**
     * @brief Uninstalles the collection item with the given index.
     * @param index Index of the item.
     * @return True if the uninstallation was successful.
     */
    bool uninstallItem(const QModelIndex& index);

    /**
     * @brief Enables or disables the info columns.
     * @param val True enalbes the info columns, false disables them.
     */
    void setShowInfoColumns(bool val);

private:
    /// Shows info columns
    bool m_showInfoColumns;

    /// Root item
    GtpyCollapsibleLocalItem* m_rootItem;
};

#endif // GTPYCOLLECTIONLOCALMODEL_H
