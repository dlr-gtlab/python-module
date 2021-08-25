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

/**
 * @brief The GtpyCollectionBrowser class
 */
class GtpyCollectionBrowser : public GtAbstractBrowserWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionBrowser
     * @param parent
     */
    Q_INVOKABLE GtpyCollectionBrowser(QWidget* parent = Q_NULLPTR);

    /**
     * @brief setItems
     * @param installedItems
     * @param availableItems
     * @param updateAvailableItems
     */
    void setItems(const QList<GtCollectionNetworkItem>& installedItems,
                  const QList<GtCollectionNetworkItem>& availableItems,
                  const QList<GtCollectionNetworkItem>& updateAvailableItems);

    /**
     * @brief itemsToDownload
     * @return
     */
    QList<GtCollectionNetworkItem> itemsToDownload() Q_DECL_OVERRIDE;

    /**
     * @brief itemsToUpdate
     * @return
     */
    QList<GtCollectionNetworkItem> itemsToUpdate() Q_DECL_OVERRIDE;

    /**
     * @brief selectAllItems
     */
    void selectAllItems() Q_DECL_OVERRIDE;

    /**
     * @brief unselectAllItems
     */
    void unselectAllItems() Q_DECL_OVERRIDE;

private:
    /// collection browser model.
    GtpyCollectionBrowserModel* m_model;

    /// Tree view
    QTreeView* m_view;

private slots:
    /**
     * @brief onItemClicked
     * @param index
     */
    void onItemClicked(const QModelIndex& index);

    /**
     * @brief onItemSelectionChange
     */
    void onItemSelectionChange();
};

#endif // GTPYCOLLECTIONBROWSER_H
