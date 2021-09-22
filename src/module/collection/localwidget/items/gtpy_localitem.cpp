/* GTlab - Gas Turbine laboratory
 * Source File: gtpylocalitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_localitem.h"

GtpyLocalItem::GtpyLocalItem(const GtCollectionItem& item) :
    m_item(item)
{

}

bool GtpyLocalItem::isCollapsible() const
{
    return false;
}

QString GtpyLocalItem::ident() const
{
    return m_item.ident();
}

QString GtpyLocalItem::version() const
{
    return QString::number(m_item.version());
}

QString GtpyLocalItem::description() const
{
    return m_item.description();
}

GtCollectionItem
GtpyLocalItem::item() const
{
    return m_item;
}
