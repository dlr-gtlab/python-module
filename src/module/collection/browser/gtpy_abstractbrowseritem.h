/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractbrowseritem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYABSTRACTBROWSERITEM_H
#define GTPYABSTRACTBROWSERITEM_H

#include <QVector>
#include <QString>

#include "gt_collectionnetworkitem.h"

class GtpyAbstractBrowserItem
{
public:
    GtpyAbstractBrowserItem();

    virtual ~GtpyAbstractBrowserItem();

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

    GtpyAbstractBrowserItem* child(int row);

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

    GtpyAbstractBrowserItem* parentItem() const;

    void setParentItem(GtpyAbstractBrowserItem* parent);

    void setTypeId(int typeId);

    int typeId() const;

protected:
    QVector<GtpyAbstractBrowserItem*> m_childItems;

private:
    /// Item selected indicator
    bool m_selected;

    GtpyAbstractBrowserItem* m_parentItem;

    int m_typeId;
};

#endif // GTPYABSTRACTBROWSERITEM_H
