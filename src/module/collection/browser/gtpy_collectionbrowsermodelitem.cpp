/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodelitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_collectionbrowsermodelitem.h"

GtpyCollectionBrowserModelItem::GtpyCollectionBrowserModelItem(
    const GtCollectionNetworkItem& item) : m_item(item), m_selected(false)
{

}

GtCollectionNetworkItem
GtpyCollectionBrowserModelItem::item() const
{
    return m_item;
}

bool
GtpyCollectionBrowserModelItem::isSelected() const
{
    return m_selected;
}

void
GtpyCollectionBrowserModelItem::setSelected(bool selected)
{
    m_selected = selected;
}

