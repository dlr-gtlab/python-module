/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_rootbrowseritem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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
    GtpyCollapsibleBrowserItem* item = Q_NULLPTR;

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
    GtpyCollapsibleBrowserItem* item = Q_NULLPTR;

    if (m_itemTypes.contains(typeId))
    {
        foreach (GtpyAbstractBrowserItem* child, m_childItems)
        {
            if (child->typeId() == typeId)
            {
                item = dynamic_cast<GtpyCollapsibleBrowserItem*>(child);
                break;
            }
        }
    }

    return item;
}

void
GtpyRootBrowserItem::clearRoot()
{
    m_itemTypes.clear();
    qDeleteAll(m_childItems);
    m_childItems.clear();
}
