/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectioncollapsibleitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 22.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_collectioncollapsibleitem.h"

GtpyCollectionCollapsibleItem::GtpyCollectionCollapsibleItem(QString ident) :
    m_ident(ident)
{
    setSelected(false);
}

bool
GtpyCollectionCollapsibleItem::isCollapsible() const
{
    return true;
}

QString
GtpyCollectionCollapsibleItem::ident() const
{
    return m_ident;
}

GtpyCollectionCollapsibleItem*
GtpyCollectionCollapsibleItem::collapsibleChild(const QString& ident)
{
    foreach (GtpyAbstractCollectionItem* item, m_childItems)
    {
        if (item->ident() == ident && item->isCollapsible())
        {
            return dynamic_cast<GtpyCollectionCollapsibleItem*>(item);
        }
    }

    return Q_NULLPTR;
}

void
GtpyCollectionCollapsibleItem::appendChild(GtpyAbstractCollectionItem* item)
{
    m_childItems.append(item);
}

void
GtpyCollectionCollapsibleItem::appendChild(GtpyAbstractCollectionItem* item,
        QStringList hierarchy)
{
    GtpyCollectionCollapsibleItem* level = this;

    foreach (QString itemName, hierarchy)
    {
        GtpyCollectionCollapsibleItem* child =
            level->collapsibleChild(itemName);

        if (child == Q_NULLPTR)
        {
            child = new GtpyCollectionCollapsibleItem(itemName);
            level->appendChild(child);
        }

        level = child;
    }

    level->appendChild(item);
}


