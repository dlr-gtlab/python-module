/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowser.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONBROWSER_H
#define GTPYCOLLECTIONBROWSER_H

#include "gt_abstractbrowserwidget.h"
#include "gt_collectionnetworkitem.h"

class QTreeView;

class GtpyCollectionBrowserModel;
class GtpyCollectionBrowserSortModel;

/**
 * @brief The GtpyCollectionBrowser class
 */
class GtpyCollectionBrowser : public GtAbstractBrowserWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionBrowser
     * @param parent Parent widget.
     */
    explicit GtpyCollectionBrowser(QWidget* parent = Q_NULLPTR);

    /**
     * @brief Appends the given list of items as children of the root
     * element.
     * @param installedItems List of installed items.
     * @param availableItems List of available items.
     * @param updateAvailableItems List of update available items.
     */
    void setItems(const QList<GtCollectionNetworkItem>& installedItems,
                  const QList<GtCollectionNetworkItem>& availableItems,
                  const QList<GtCollectionNetworkItem>& updateAvailableItems);

    /**
     * @brief Returns a list of the items to download.
     * @return A list of the items to download.
     */
    QList<GtCollectionNetworkItem> itemsToDownload() Q_DECL_OVERRIDE;

    /**
     * @brief Returns a list of the itmes to update.
     * @return A list of the items to update.
     */
    QList<GtCollectionNetworkItem> itemsToUpdate() Q_DECL_OVERRIDE;

    /**
     * @brief Selects all items.
     */
    void selectAllItems() Q_DECL_OVERRIDE;

    /**
     * @brief Unselect all items.
     */
    void unselectAllItems() Q_DECL_OVERRIDE;

private:
    /**
     * @brief Returns the source model index corresponding to the given index
     * from the sorting model.
     * @param index Index of the sorting model.
     * @return The source model index corresponding to the given index.
     */
    QModelIndex mapToSource(const QModelIndex& index) const;

    /// collection browser model.
    GtpyCollectionBrowserModel* m_model;

    /// Sorting model
    GtpyCollectionBrowserSortModel* m_sortModel;

    /// Tree view
    QTreeView* m_view;

private slots:
    /**
     * @brief Emitted after item was selected. Returns selected item.
     * @param Selected item.
     */
    void onItemClicked(const QModelIndex& index);

    /**
     * @brief Emitted after item selection changed.
     */
    void onItemSelectionChange();
};

#endif // GTPYCOLLECTIONBROWSER_H
