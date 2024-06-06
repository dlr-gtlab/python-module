/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionlocalwidget.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 16.09.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCOLLECTIONLOCALWIDGET_H
#define GTPYCOLLECTIONLOCALWIDGET_H

#include <QWidget>
#include <QModelIndex>

class QTreeView;
class QSortFilterProxyModel;

class GtCollectionItem;
class GtpyCollectionLocalModel;

/**
 * @brief The GtpyCollectionLocalWidget class
 */
class GtpyCollectionLocalWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionLocalWidget
     * @param parent Parent widget.
     */
    explicit GtpyCollectionLocalWidget(QWidget* parent = nullptr);

    /**
       * @brief Passes the given list of items to the model.
       * @param items List of items.
       */
    void setItems(const QList<GtCollectionItem>& items);

    /**
     * @brief Retruns the item at the given index.
     * @param index Index of the item.
     * @return The item at the given index.
     */
    GtCollectionItem getItem(const QModelIndex& index);

    /**
     * @brief Enables or disables the context menu.
     * @param val True enalbes the context menu, false disables it.
     */
    void setCustomContextMenuEnabled(bool val);

    /**
     * @brief Enables or disables the item infos.
     * @param val True enalbes the item infos, false disables it.
     */
    void setShowItemInfo(bool val);

private:
    /// Tree view.
    QTreeView* m_view;

    /// Local collection model.
    GtpyCollectionLocalModel* m_model;

    /// Sorting model
    QSortFilterProxyModel* m_sortModel;

    /// Enables custom context menu
    bool m_contextMenuEnabled;

    /**
     * @brief Returns the source model index corresponding to the given index
     * from the sorting model.
     * @param index Index of the sorting model.
     * @return The source model index corresponding to the given index.
     */
    QModelIndex mapToSource(const QModelIndex& index) const;

private slots:
    /**
     * @brief It opens the script corresponding to the given index and displays
     * it on the left side of the local collection view. If the column with the
     * given index is equal to 1, the info widget will be opened additionally.
     * @param index Index of the local collection model.
     */
    void onItemClicked(const QModelIndex& index);

    /**
     * @brief Opens a context menu that provides the uninstall functionality.
     * @param pos Current cursor position.
     */
    void onCustomContextMenuRequested(const QPoint& pos);

    /**
     * @brief Emits the itemSelected signal.
     * @param current Current selected item.
     * @param previous Previous selected item.
     */
    void onCurrentChanged(const QModelIndex& current,
                          const QModelIndex& previous = QModelIndex());

signals:
    /**
     * @brief Emitted after item was selected. Returns selected item.
     * @param Selected item.
     */
    void itemSelected(const QModelIndex& index);

    /**
     * @brief Emitted after item was double clicked. Returns double clicked
     * item.
     * @param index Double clicked item.
     */
    void itemDoubleClicked(const QModelIndex& index);

};

#endif // GTPYCOLLECTIONLOCALWIDGET_H
