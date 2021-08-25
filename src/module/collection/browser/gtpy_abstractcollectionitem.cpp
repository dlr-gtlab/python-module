/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractcollectionitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_abstractcollectionitem.h"

GtpyAbstractCollectionItem::GtpyAbstractCollectionItem() : m_parentItem(
        Q_NULLPTR), m_typeId(-1)
{

}

GtpyAbstractCollectionItem::~GtpyAbstractCollectionItem()
{
    qDeleteAll(m_childItems);
}

void
GtpyAbstractCollectionItem::selectAllChildren()
{
    return;
}

void
GtpyAbstractCollectionItem::unselectAllChildren()
{
    return;
}

QList<GtCollectionNetworkItem>
GtpyAbstractCollectionItem::uncollapsibleChilren()
{
    return QList<GtCollectionNetworkItem>();
}

QList<GtCollectionNetworkItem>
GtpyAbstractCollectionItem::selectedItems()
{
    return QList<GtCollectionNetworkItem>();
}

QString
GtpyAbstractCollectionItem::version() const
{
    return QString();
}

QString
GtpyAbstractCollectionItem::installedVersion() const
{
    return QString();
}

GtCollectionNetworkItem
GtpyAbstractCollectionItem::item() const
{
    return GtCollectionNetworkItem();
}

int
GtpyAbstractCollectionItem::childCount() const
{
    return m_childItems.count();
}

GtpyAbstractCollectionItem*
GtpyAbstractCollectionItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
    {
        return Q_NULLPTR;
    }

    return m_childItems.at(row);
}

int
GtpyAbstractCollectionItem::row() const
{
    if (m_parentItem)
    {
        return m_parentItem->m_childItems.indexOf(
                   const_cast<GtpyAbstractCollectionItem*>(this));
    }

    return 0;
}

bool
GtpyAbstractCollectionItem::isSelected() const
{
    return m_selected;
}

void
GtpyAbstractCollectionItem::setSelected(bool selected)
{
    m_selected = selected;
}

GtpyAbstractCollectionItem*
GtpyAbstractCollectionItem::parentItem() const
{
    return m_parentItem;
}

void
GtpyAbstractCollectionItem::setParentItem(GtpyAbstractCollectionItem* parent)
{
    m_parentItem = parent;
}

void
GtpyAbstractCollectionItem::setTypeId(int typeId)
{
    m_typeId = typeId;
}

int
GtpyAbstractCollectionItem::typeId() const
{
    return m_typeId;
}
