/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_browseritem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.08.2021
 * Author: Marvin Noethen (DLR AT-TWK)
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
