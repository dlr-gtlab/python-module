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
    GtpyAbstractCollectionItem(), m_ident(ident)
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

void
GtpyCollectionCollapsibleItem::selectAllChildren()
{
    foreach (GtpyAbstractCollectionItem* item, m_childItems)
    {
        if (item->isCollapsible())
        {
            item->selectAllChildren();
        }
        else
        {
            item->setSelected(true);
        }
    }
}

void
GtpyCollectionCollapsibleItem::unselectAllChildren()
{
    foreach (GtpyAbstractCollectionItem* item, m_childItems)
    {
        if (item->isCollapsible())
        {
            item->unselectAllChildren();
        }
        else
        {
            item->setSelected(false);
        }
    }
}

QList<GtCollectionNetworkItem>
GtpyCollectionCollapsibleItem::uncollapsibleChilren()
{
    QList<GtCollectionNetworkItem> retval;

    foreach (GtpyAbstractCollectionItem* item, m_childItems)
    {
        if (item->isCollapsible())
        {
            retval.append(item->uncollapsibleChilren());
        }
        else
        {
            retval.append(item->item());
        }
    }

    return retval;
}

QList<GtCollectionNetworkItem>
GtpyCollectionCollapsibleItem::selectedItems()
{
    QList<GtCollectionNetworkItem> retval;

    foreach (GtpyAbstractCollectionItem* item, m_childItems)
    {
        if (item->isCollapsible())
        {
            retval.append(item->selectedItems());
        }
        else
        {
            if (item->isSelected())
            {
                retval.append(item->item());
            }
        }
    }

    return retval;
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
    item->setParentItem(this);
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
            child->setTypeId(typeId());
            level->appendChild(child);
        }

        level = child;
    }

    item->setTypeId(typeId());
    level->appendChild(item);
}
