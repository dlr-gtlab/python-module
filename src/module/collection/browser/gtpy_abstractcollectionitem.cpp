/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractcollectionitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_abstractcollectionitem.h"

GtpyAbstractCollectionItem::GtpyAbstractCollectionItem()
{

}

GtpyAbstractCollectionItem::~GtpyAbstractCollectionItem()
{
    qDeleteAll(m_childItems);
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
