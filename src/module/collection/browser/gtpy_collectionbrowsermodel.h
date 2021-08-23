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

#include "gtpy_abstractcollectionitem.h"

class GtpyCollectionBrowserModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum CollectionItemType
    {
        InstalledItem = 1,
        AvailableItem = 2,
        UpdateAvailableItem = 3,
        InstalledItemGroup = 4,
        AvailableItemGroup = 5,
        UpdateAvailableItemGroup = 6
    };

    GtpyCollectionBrowserModel(QObject* parent = Q_NULLPTR);

    ~GtpyCollectionBrowserModel();

    /**
     * @brief rowCount
     * @param parent
     * @return
     */
    int rowCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;

    /**
     * @brief columnCount
     * @param parent
     * @return
     */
    int columnCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;

    /**
     * @brief data
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

    /**
     * @brief setData
     * @param index
     * @param value
     * @param role
     * @return
     */
    bool setData(const QModelIndex& index,
                 const QVariant& value, int role) Q_DECL_OVERRIDE;

    /**
     * @brief headerData
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

    /**
     * @brief setCollectionData
     * @param installedItems
     * @param availableItems
     * @param updataAvailableItems
     */
    void setCollectionData(const QList<GtCollectionNetworkItem>& installedItems,
                           const QList<GtCollectionNetworkItem>& availableItems,
                           const QList<GtCollectionNetworkItem>& updataAvailableItems);

    /**
     * @brief index
     * @param row
     * @param column
     * @param parent
     * @return
     */
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    /**
     * @brief parent
     * @param index
     * @return
     */
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief flags
     * @param index
     * @return
     */
    virtual Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

    /**
     * @brief itemFromIndex
     * @param index
     * @return
     */
    GtCollectionItem itemFromIndex(const QModelIndex& index);

    /**
     * @brief selectedItems
     * @return
     */
    QList<GtCollectionNetworkItem> selectedItems();

    /**
     * @brief itemsToUpdate
     * @return
     */
    QList<GtCollectionNetworkItem> itemsToUpdate();

    /**
     * @brief selectAll
     */
    void selectAll();

    /**
     * @brief unselectAll
     */
    void unselectAll();

private:
    // Installed collection items.
    QList<GtpyAbstractCollectionItem*> m_installedItems;

    // Collection items available for installation.
    QList<GtpyAbstractCollectionItem*> m_availableItems;

    // Collection items with update available.
    QList<GtpyAbstractCollectionItem*> m_updateAvailableItems;

    /**
     * @brief groupTypes
     * @return
     */
    QList<CollectionItemType> groupTypes() const;

signals:
    /**
     * @brief selectionChanged
     */
    void selectionChanged();
};

#endif // GTPYCOLLECTIONBROWSERMODEL_H
