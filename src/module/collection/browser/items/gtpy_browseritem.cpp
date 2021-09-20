/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_browseritem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_browseritem.h"

GtpyBrowserItem::GtpyBrowserItem(const GtCollectionNetworkItem& item) :
    GtpyAbstractBrowserItem(), m_item(item)
{
    setSelected(false);
}

bool
GtpyBrowserItem::isCollapsible() const
{
    return false;
}

QString
GtpyBrowserItem::ident() const
{
    return m_item.ident();
}

QString
GtpyBrowserItem::version() const
{
    return QString::number(m_item.version());
}

QString
GtpyBrowserItem::installedVersion() const
{
    return QString::number(m_item.installedVersion());
}

GtCollectionNetworkItem
GtpyBrowserItem::item() const
{
    return m_item;
}
