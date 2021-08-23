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
