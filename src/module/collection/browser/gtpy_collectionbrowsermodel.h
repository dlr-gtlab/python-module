/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodel.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONBROWSERMODEL_H
#define GTPYCOLLECTIONBROWSERMODEL_H

#include <QAbstractItemModel>

#include "gtpy_abstractbrowseritem.h"

class GtpyRootBrowserItem;
class GtpyCollapsibleBrowserItem;

/**
 * @brief The GtpyCollectionBrowserModel class
 */
class GtpyCollectionBrowserModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum GtpyCollectionItemType
    {
        InstalledItem = 1,
        AvailableItem = 2,
        UpdateAvailableItem = 3
    };

    /**
     * @brief GtpyCollectionBrowserModel
     * @param parent Parent object.
     */
    GtpyCollectionBrowserModel(QObject* parent = Q_NULLPTR);

    /**
     * @brief ~GtpyCollectionBrowserModel
     */
    virtual ~GtpyCollectionBrowserModel();

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
     * @brief Set the item with the given index checked or unchecked.
     * @param index Index of an item.
     * @param value Whether it is checked or not.
     * @param role Type of data that the function receives. If it is not
     * CheckStateRole, the default implementation is called.
     * @return
     */
    bool setData(const QModelIndex& index,
                 const QVariant& value, int role) Q_DECL_OVERRIDE;

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
     * @brief setCollectionData
     * @param installedItems
     * @param availableItems
     * @param updataAvailableItems
     */
    void setCollectionData(
        const QList<GtCollectionNetworkItem>& installedItems,
        const QList<GtCollectionNetworkItem>& availableItems,
        const QList<GtCollectionNetworkItem>& updataAvailableItems);

    /**
     * @brief Returns the index of the item in the model.
     * @param row Row of the item.
     * @param column Column of the item.
     * @param parent Parent of the item.
     * @return Index of the item in the model.
     */
    QModelIndex index(int row, int column, const QModelIndex& parent =
                          QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the parent index of the item with the given index.
     * @param index Index of an item of the model.
     * @return The parent index of the item with the given index.
     */
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns the item flags for the given index.
     * @param index Index of an item.
     * @return The item flags for the given index.
     */
    virtual Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief Retruns the collection item with the given index.
     * @param index Index of the item.
     * @return The collection item with the given index.
     */
    GtCollectionItem itemFromIndex(const QModelIndex& index);

    /**
     * @brief Returns a list of the selected items.
     * @return A list of the selected items.
     */
    QList<GtCollectionNetworkItem> selectedItems();

    /**
     * @brief Retruns a list of the items to update.
     * @return A list of the items to update.
     */
    QList<GtCollectionNetworkItem> itemsToUpdate();

    /**
     * @brief Selects all items.
     */
    void selectAll();

    /**
     * @brief Unselects all items.
     */
    void unselectAll();

private:
    /**
     * @brief Appends the given item to the given collapsible item.
     * @param item Item to append.
     * @param to Collapsible parent item.
     */
    void appendItemTo(GtCollectionNetworkItem item ,
                      GtpyCollapsibleBrowserItem* to);

    /// Root item.
    GtpyRootBrowserItem* m_rootItem;

signals:
    /**
     * @brief selectionChanged
     */
    void selectionChanged();
};

#endif // GTPYCOLLECTIONBROWSERMODEL_H
