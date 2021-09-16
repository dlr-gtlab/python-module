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

    virtual QString version() const;

    virtual QString installedVersion() const;

    virtual QString description() const;

    virtual GtCollectionNetworkItem item() const;

    virtual void selectAllChildren();

    virtual void unselectAllChildren();

    virtual QList<GtCollectionNetworkItem> selectedItems();

    virtual QList<GtCollectionNetworkItem> uncollapsibleChilren();

    int childCount() const;

    GtpyAbstractBrowserItem* child(int row);

    void deleteChildren();

    int row() const;

    GtpyAbstractBrowserItem* parentItem() const;

    void setParentItem(GtpyAbstractBrowserItem* parent);

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

    void setTypeId(int typeId);

    int typeId() const;

protected:
    QVector<GtpyAbstractBrowserItem*> m_childItems;

private:
    GtpyAbstractBrowserItem* m_parentItem;

    /// Item selected indicator
    bool m_selected;

    int m_typeId;
};

#endif // GTPYABSTRACTBROWSERITEM_H
