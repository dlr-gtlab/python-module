/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collapsiblelocalitem.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 16.09.2021
 * Author: Marvin Noethen (AT-TW)
 */

#include "gtpy_collapsiblelocalitem.h"

GtpyCollapsibleLocalItem::GtpyCollapsibleLocalItem(QString const& ident) :
    m_ident(ident)
{

}

bool
GtpyCollapsibleLocalItem::isCollapsible() const
{
    return true;
}

QString
GtpyCollapsibleLocalItem::ident() const
{
    return m_ident;
}

void
GtpyCollapsibleLocalItem::appendChild(GtpyAbstractLocalItem* item,
                                      QStringList hierarchy)
{
    GtpyCollapsibleLocalItem* level = this;

    foreach (QString itemName, hierarchy)
    {
        GtpyCollapsibleLocalItem* child =
            level->collapsibleChild(itemName);

        if (child == Q_NULLPTR)
        {
            child = new GtpyCollapsibleLocalItem(itemName);
            level->appendChild(child);
        }

        level = child;
    }

    level->appendChild(item);
}

void
GtpyCollapsibleLocalItem::appendChild(GtpyAbstractLocalItem* item)
{
    item->setParentItem(this);
    m_childItems.append(item);
}

GtpyCollapsibleLocalItem*
GtpyCollapsibleLocalItem::collapsibleChild(const QString& ident)
{
    auto iter = std::find_if(std::begin(m_childItems),
                             std::end(m_childItems),
                             [&ident](const auto* item)
    {
        return item->ident() == ident && item->isCollapsible();
    });

    if (iter == m_childItems.end()) return nullptr;

    return dynamic_cast<GtpyCollapsibleLocalItem*>(*iter);
}
