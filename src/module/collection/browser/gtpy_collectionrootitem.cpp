/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowserrootitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_collectioncollapsibleitem.h"

#include "gtpy_collectionrootitem.h"

GtpyCollectionRootItem::GtpyCollectionRootItem() : GtpyAbstractCollectionItem()
{

}

GtpyCollectionRootItem::~GtpyCollectionRootItem()
{
    clearRoot();
}

bool
GtpyCollectionRootItem::isCollapsible() const
{
    return false;
}

QString
GtpyCollectionRootItem::ident() const
{
    return "Root";
}

GtpyCollectionCollapsibleItem*
GtpyCollectionRootItem::createChild(int typeId, const QString& ident)
{
    GtpyCollectionCollapsibleItem* item = Q_NULLPTR;

    if (!m_itemTypes.contains(typeId))
    {
        item = new GtpyCollectionCollapsibleItem(
            ident);
        item->setParentItem(this);
        item->setTypeId(typeId);
        m_childItems.append(item);

        m_itemTypes.append(typeId);
    }

    return item;
}

GtpyCollectionCollapsibleItem*
GtpyCollectionRootItem::child(int typeId)
{
    GtpyCollectionCollapsibleItem* item = Q_NULLPTR;

    if (m_itemTypes.contains(typeId))
    {
        foreach (GtpyAbstractCollectionItem* child, m_childItems)
        {
            if (child->typeId() == typeId)
            {
                item = dynamic_cast<GtpyCollectionCollapsibleItem*>(child);
                break;
            }
        }
    }

    return item;
}

void
GtpyCollectionRootItem::clearRoot()
{
    m_itemTypes.clear();
    qDeleteAll(m_childItems);
    m_childItems.clear();
}
