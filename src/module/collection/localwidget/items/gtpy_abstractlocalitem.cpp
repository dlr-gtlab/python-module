/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractlocalitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_abstractlocalitem.h"

GtpyAbstractLocalItem::GtpyAbstractLocalItem() : m_parentItem(Q_NULLPTR)
{

}

GtpyAbstractLocalItem::~GtpyAbstractLocalItem()
{
    deleteChildren();
}

QString
GtpyAbstractLocalItem::version() const
{
    return QString();
}

QString
GtpyAbstractLocalItem::installedVersion() const
{
    return QString();
}

QString
GtpyAbstractLocalItem::description() const
{
    return QString();
}

GtCollectionItem
GtpyAbstractLocalItem::item() const
{
    return GtCollectionItem();
}

int
GtpyAbstractLocalItem::childCount() const
{
    return m_childItems.count();
}

GtpyAbstractLocalItem*
GtpyAbstractLocalItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
    {
        return Q_NULLPTR;
    }

    return m_childItems.at(row);
}

bool
GtpyAbstractLocalItem::deleteChild(int row)
{
    if (row < 0 || row >= m_childItems.size())
    {
        return false;
    }

    GtpyAbstractLocalItem* child = m_childItems.takeAt(row);

    delete child;
    child = Q_NULLPTR;

    Q_UNUSED(child);

    return true;
}

void
GtpyAbstractLocalItem::deleteChildren()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

int
GtpyAbstractLocalItem::row() const
{
    if (m_parentItem)
    {
        return m_parentItem->m_childItems.indexOf(
                   const_cast<GtpyAbstractLocalItem*>(this));
    }

    return 0;
}

GtpyAbstractLocalItem*
GtpyAbstractLocalItem::parentItem() const
{
    return m_parentItem;
}

void
GtpyAbstractLocalItem::setParentItem(GtpyAbstractLocalItem* parent)
{
    m_parentItem = parent;
}


