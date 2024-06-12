/* GTlab - Gas Turbine laboratory
 * Source File: gtpylocalitem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 16.09.2021
 * Author: Marvin Noethen (DLR AT-TWK)
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
