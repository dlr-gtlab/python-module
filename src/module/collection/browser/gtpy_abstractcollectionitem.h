/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractcollectionitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYABSTRACTCOLLECTIONITEM_H
#define GTPYABSTRACTCOLLECTIONITEM_H

#include <QVector>
#include <QString>

#include "gt_collectionnetworkitem.h"

class GtpyAbstractCollectionItem
{
public:
    GtpyAbstractCollectionItem();

    virtual ~GtpyAbstractCollectionItem();

    virtual bool isCollapsible() const = 0;

    virtual QString ident() const = 0;

    virtual void selectAllChildren();

    virtual void unselectAllChildren();

    virtual QList<GtCollectionNetworkItem> uncollapsibleChilren();

    virtual QList<GtCollectionNetworkItem> selectedItems();

    virtual QString version() const;

    virtual QString installedVersion() const;

    virtual GtCollectionNetworkItem item() const;

    int childCount() const;

    GtpyAbstractCollectionItem* child(int row);

    int row() const;

    /**
     * @brief isSelected
     * @return
     */
    bool isSelected() const;

    /**
     * @brief setSelected
     * @param selected
     */
    void setSelected(bool selected);

    GtpyAbstractCollectionItem* parentItem() const;

    void setParentItem(GtpyAbstractCollectionItem* parent);

    void setTypeId(int typeId);

    int typeId() const;

protected:
    QVector<GtpyAbstractCollectionItem*> m_childItems;

private:
    /// Item selected indicator
    bool m_selected;

    GtpyAbstractCollectionItem* m_parentItem;

    int m_typeId;
};

#endif // GTPYABSTRACTCOLLECTIONITEM_H
