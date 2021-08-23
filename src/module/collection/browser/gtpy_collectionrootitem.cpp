/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowserrootitem.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QTranslator>

#include "gtpy_collectioncollapsibleitem.h"

#include "gtpy_collectionrootitem.h"

GtpyCollectionRootItem::GtpyCollectionRootItem() : GtpyAbstractCollectionItem(),
    m_installed(Q_NULLPTR), m_available(Q_NULLPTR), m_updateAvailable(Q_NULLPTR)
{
    reinit();
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

void
GtpyCollectionRootItem::reinit()
{
    clearRoot();

    m_installed = new GtpyCollectionCollapsibleItem(
        QTranslator::tr("Installed"));
    m_installed->setParentItem(this);
    m_childItems.append(m_installed);

    m_available = new GtpyCollectionCollapsibleItem(
        QTranslator::tr("Available"));
    m_available->setParentItem(this);
    m_childItems.append(m_available);

    m_updateAvailable = new GtpyCollectionCollapsibleItem(
        QTranslator::tr("Update available!"));
    m_updateAvailable->setParentItem(this);
    m_childItems.append(m_updateAvailable);
}

GtpyCollectionCollapsibleItem*
GtpyCollectionRootItem::installed() const
{
    return m_installed;
}

GtpyCollectionCollapsibleItem*
GtpyCollectionRootItem::available() const
{
    return m_available;
}

GtpyCollectionCollapsibleItem*
GtpyCollectionRootItem::updateAvailable() const
{
    return m_updateAvailable;
}

void
GtpyCollectionRootItem::clearRoot()
{
    if (m_installed)
    {
        delete m_installed;
        m_installed = Q_NULLPTR;
    }

    if (m_available)
    {
        delete m_available;
        m_available = Q_NULLPTR;
    }

    if (m_updateAvailable)
    {
        delete m_updateAvailable;
        m_updateAvailable = Q_NULLPTR;
    }

    m_childItems.clear();
}
