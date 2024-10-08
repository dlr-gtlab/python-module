/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collapsiblebrowseritem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.08.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gtpy_collapsiblebrowseritem.h"

#include <algorithm>

GtpyCollapsibleBrowserItem::GtpyCollapsibleBrowserItem(QString const& ident) :
    GtpyAbstractBrowserItem(), m_ident(ident)
{
    setSelected(false);
}

bool
GtpyCollapsibleBrowserItem::isCollapsible() const
{
    return true;
}

QString
GtpyCollapsibleBrowserItem::ident() const
{
    return m_ident;
}

void
GtpyCollapsibleBrowserItem::selectAllChildren()
{
    foreach (GtpyAbstractBrowserItem* item, m_childItems)
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
GtpyCollapsibleBrowserItem::unselectAllChildren()
{
    foreach (GtpyAbstractBrowserItem* item, m_childItems)
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
GtpyCollapsibleBrowserItem::uncollapsibleChilren()
{
    QList<GtCollectionNetworkItem> retval;

    foreach (GtpyAbstractBrowserItem* item, m_childItems)
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
GtpyCollapsibleBrowserItem::selectedItems()
{
    QList<GtCollectionNetworkItem> retval;

    foreach (GtpyAbstractBrowserItem* item, m_childItems)
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

GtpyCollapsibleBrowserItem*
GtpyCollapsibleBrowserItem::collapsibleChild(const QString& ident)
{
    auto iter = std::find_if(std::begin(m_childItems),
                             std::end(m_childItems),
                             [&ident](const auto* item)
    {
        return item->ident() == ident && item->isCollapsible();
    });

    if (iter == m_childItems.end()) return nullptr;

    return dynamic_cast<GtpyCollapsibleBrowserItem*>(*iter);
}

void
GtpyCollapsibleBrowserItem::appendChild(GtpyAbstractBrowserItem* item)
{
    item->setParentItem(this);
    m_childItems.append(item);
}

void
GtpyCollapsibleBrowserItem::appendChild(GtpyAbstractBrowserItem* item,
                                        QStringList hierarchy)
{
    GtpyCollapsibleBrowserItem* level = this;

    foreach (QString itemName, hierarchy)
    {
        GtpyCollapsibleBrowserItem* child =
            level->collapsibleChild(itemName);

        if (child == nullptr)
        {
            child = new GtpyCollapsibleBrowserItem(itemName);
            child->setTypeId(typeId());
            level->appendChild(child);
        }

        level = child;
    }

    item->setTypeId(typeId());
    level->appendChild(item);
}
