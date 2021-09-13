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

    GtpyCollectionCollapsibleItem* createChild(int typeId, const QString& ident);

    GtpyCollectionCollapsibleItem* child(int typeId);

    //    GtpyCollectionCollapsibleItem* createInstalled(int typeId);

    //    GtpyCollectionCollapsibleItem* installed() const;

    //    GtpyCollectionCollapsibleItem* createAvailable(int typeId);

    //    GtpyCollectionCollapsibleItem* available() const;

    //    GtpyCollectionCollapsibleItem* createUpdateAvailable(int typeId);

    //    GtpyCollectionCollapsibleItem* updateAvailable() const;

    void clearRoot();
private:
    //    void clearRoot();

    //    GtpyCollectionCollapsibleItem* m_installed;

    //    GtpyCollectionCollapsibleItem* m_available;

    //    GtpyCollectionCollapsibleItem* m_updateAvailable;

    QList<int> m_itemTypes;
};

#endif // GTPYCOLLECTIONROOTITEM_H
