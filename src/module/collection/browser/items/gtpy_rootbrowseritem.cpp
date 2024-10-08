/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_rootbrowseritem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 23.08.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_collapsiblebrowseritem.h"

#include "gtpy_rootbrowseritem.h"

GtpyRootBrowserItem::GtpyRootBrowserItem() : GtpyAbstractBrowserItem()
{

}

GtpyRootBrowserItem::~GtpyRootBrowserItem()
{
    clearRoot();
}

bool
GtpyRootBrowserItem::isCollapsible() const
{
    return false;
}

QString
GtpyRootBrowserItem::ident() const
{
    return "Root";
}

GtpyCollapsibleBrowserItem*
GtpyRootBrowserItem::createChild(int typeId, const QString& ident)
{
    GtpyCollapsibleBrowserItem* item = nullptr;

    if (!m_itemTypes.contains(typeId))
    {
        item = new GtpyCollapsibleBrowserItem(
            ident);
        item->setParentItem(this);
        item->setTypeId(typeId);
        m_childItems.append(item);

        m_itemTypes.append(typeId);
    }

    return item;
}

GtpyCollapsibleBrowserItem*
GtpyRootBrowserItem::child(int typeId)
{
    if (!m_itemTypes.contains(typeId)) return nullptr;

    auto iter = std::find_if(std::begin(m_childItems),
                             std::end(m_childItems),
                             [typeId](const auto* child)
    {
        return child->typeId() == typeId;
    });

    if (iter == m_childItems.end()) return nullptr;

    return dynamic_cast<GtpyCollapsibleBrowserItem*>(*iter);
}

void
GtpyRootBrowserItem::clearRoot()
{
    m_itemTypes.clear();
    qDeleteAll(m_childItems);
    m_childItems.clear();
}
