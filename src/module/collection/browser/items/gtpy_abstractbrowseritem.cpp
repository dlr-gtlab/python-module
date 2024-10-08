/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractbrowseritem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.08.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_abstractbrowseritem.h"

GtpyAbstractBrowserItem::GtpyAbstractBrowserItem() : m_parentItem(
        nullptr), m_selected(false), m_typeId(-1)
{

}

GtpyAbstractBrowserItem::~GtpyAbstractBrowserItem()
{
    deleteChildren();
}

QString
GtpyAbstractBrowserItem::version() const
{
    return QString();
}

QString
GtpyAbstractBrowserItem::installedVersion() const
{
    return QString();
}

QString
GtpyAbstractBrowserItem::description() const
{
    return QString();
}

GtCollectionNetworkItem
GtpyAbstractBrowserItem::item() const
{
    return GtCollectionNetworkItem();
}

void
GtpyAbstractBrowserItem::selectAllChildren()
{
    return;
}

void
GtpyAbstractBrowserItem::unselectAllChildren()
{
    return;
}

QList<GtCollectionNetworkItem>
GtpyAbstractBrowserItem::selectedItems()
{
    return QList<GtCollectionNetworkItem>();
}

QList<GtCollectionNetworkItem>
GtpyAbstractBrowserItem::uncollapsibleChilren()
{
    return QList<GtCollectionNetworkItem>();
}

int
GtpyAbstractBrowserItem::childCount() const
{
    return m_childItems.count();
}

GtpyAbstractBrowserItem*
GtpyAbstractBrowserItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
    {
        return nullptr;
    }

    return m_childItems.at(row);
}

void
GtpyAbstractBrowserItem::deleteChildren()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();
}

int
GtpyAbstractBrowserItem::row() const
{
    if (m_parentItem)
    {
        return m_parentItem->m_childItems.indexOf(
                   const_cast<GtpyAbstractBrowserItem*>(this));
    }

    return 0;
}

GtpyAbstractBrowserItem*
GtpyAbstractBrowserItem::parentItem() const
{
    return m_parentItem;
}

void
GtpyAbstractBrowserItem::setParentItem(GtpyAbstractBrowserItem* parent)
{
    m_parentItem = parent;
}

bool
GtpyAbstractBrowserItem::isSelected() const
{
    return m_selected;
}

void
GtpyAbstractBrowserItem::setSelected(bool selected)
{
    m_selected = selected;
}

void
GtpyAbstractBrowserItem::setTypeId(int typeId)
{
    m_typeId = typeId;
}

int
GtpyAbstractBrowserItem::typeId() const
{
    return m_typeId;
}
