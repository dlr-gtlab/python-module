/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collapsiblelocalitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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
    foreach (GtpyAbstractLocalItem* item, m_childItems)
    {
        if (item->ident() == ident && item->isCollapsible())
        {
            return dynamic_cast<GtpyCollapsibleLocalItem*>(item);
        }
    }

    return Q_NULLPTR;
}
