/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowserrootitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONROOTITEM_H
#define GTPYCOLLECTIONROOTITEM_H

#include "gtpy_abstractcollectionitem.h"

class GtpyCollectionCollapsibleItem;

class GtpyCollectionRootItem : public GtpyAbstractCollectionItem
{
public:
    GtpyCollectionRootItem();

    virtual ~GtpyCollectionRootItem();

    virtual bool isCollapsible() const;

    virtual QString ident() const;

    void reinit();

    GtpyCollectionCollapsibleItem* installed() const;

    GtpyCollectionCollapsibleItem* available() const;

    GtpyCollectionCollapsibleItem* updateAvailable() const;

private:
    void clearRoot();

    GtpyCollectionCollapsibleItem* m_installed;

    GtpyCollectionCollapsibleItem* m_available;

    GtpyCollectionCollapsibleItem* m_updateAvailable;
};

#endif // GTPYCOLLECTIONROOTITEM_H
