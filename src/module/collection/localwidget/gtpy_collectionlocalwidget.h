/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionlocalwidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONLOCALWIDGET_H
#define GTPYCOLLECTIONLOCALWIDGET_H

#include <QWidget>
#include <QTreeView>

class GtCollectionItem;
class GtpyCollectionLocalModel;

class GtpyCollectionLocalWidget : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyCollectionLocalWidget(QWidget* parent = Q_NULLPTR);

    /**
       * @brief setItems
       * @param items
       */
    void setItems(const QList<GtCollectionItem>& items);

    /**
     * @brief getItem
     * @param index
     * @return
     */
    GtCollectionItem getItem(const QModelIndex& index);

    /**
     * @brief setCustomContextMenuEnabled
     * @param val
     */
    void setCustomContextMenuEnabled(bool val);

    /**
     * @brief setShowItemInfo
     * @param val
     */
    void setShowItemInfo(bool val);

private:
    /// Tree view.
    QTreeView* m_view;

    /// Local collection model.
    GtpyCollectionLocalModel* m_model;

    /// Enables custom context menu
    bool m_contextMenuEnabled;

private slots:
    /**
     * @brief onItemClicked
     * @param index
     */
    void onItemClicked(const QModelIndex& index);

    /**
     * @brief onCustomContextMenuRequested
     * @param pos
     */
    void onCustomContextMenuRequested(const QPoint& pos);

    /**
     * @brief onCurrentChanged
     * @param current
     * @param previous
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
     * @brief itemDoubleClicked
     * @param index
     */
    void itemDoubleClicked(const QModelIndex& index);

};

#endif // GTPYCOLLECTIONLOCALWIDGET_H
