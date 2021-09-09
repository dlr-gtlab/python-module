/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_localcollectionmodel.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 25.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONWIDGETMODEL_H
#define GTPYCOLLECTIONWIDGETMODEL_H

#include <QAbstractItemModel>

#include "gt_collectionitem.h"

class GtpyCollapsibleLocalItem;

class GtpyCollectionWidgetModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    GtpyCollectionWidgetModel(QObject* parent = Q_NULLPTR);

    virtual ~GtpyCollectionWidgetModel();

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
     * @brief headerData
     * @param section
     * @param orientation
     * @param role
     * @return
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

    /**
     * @brief setItems
     * @param items
     */
    void setItems(const QList<GtCollectionItem>& items);

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
     * @brief itemFromIndex
     * @param index
     * @return
     */
    GtCollectionItem itemFromIndex(const QModelIndex& index);

    /**
     * @brief uninstallItem
     * @param index
     * @return
     */
    bool uninstallItem(const QModelIndex& index);

    /**
     * @brief setShowInfoColumns
     * @param val
     */
    void setShowInfoColumns(bool val);

private:
    /// Shows info columns
    bool m_showInfoColumns;

    GtpyCollapsibleLocalItem* m_rootItem;
};

#endif // GTPYLOCALCOLLECTIONMODEL_H
