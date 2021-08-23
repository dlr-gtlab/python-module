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
