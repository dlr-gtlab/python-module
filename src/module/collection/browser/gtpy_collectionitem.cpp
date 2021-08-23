/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodelitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_collectionitem.h"

GtpyCollectionItem::GtpyCollectionItem(const GtCollectionNetworkItem& item) :
    m_item(item)
{
    setSelected(false);
}

GtpyCollectionItem::~GtpyCollectionItem()
{

}

bool
GtpyCollectionItem::isCollapsible() const
{
    return false;
}

QString
GtpyCollectionItem::ident() const
{
    return m_item.ident();
}

QString
GtpyCollectionItem::version() const
{
    return QString::number(m_item.version());
}

QString
GtpyCollectionItem::installedVersion() const
{
    return QString::number(m_item.installedVersion());
}

GtCollectionNetworkItem
GtpyCollectionItem::item() const
{
    return m_item;
}
