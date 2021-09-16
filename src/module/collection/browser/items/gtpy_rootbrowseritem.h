/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_rootbrowseritem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 23.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYROOTBROWSERITEM_H
#define GTPYROOTBROWSERITEM_H

#include "gtpy_abstractbrowseritem.h"

class GtpyCollapsibleBrowserItem;

class GtpyRootBrowserItem : public GtpyAbstractBrowserItem
{
public:
    GtpyRootBrowserItem();

    virtual ~GtpyRootBrowserItem();

    virtual bool isCollapsible() const;

    virtual QString ident() const;

    GtpyCollapsibleBrowserItem* createChild(int typeId, const QString& ident);

    GtpyCollapsibleBrowserItem* child(int typeId);

    void clearRoot();
private:

    QList<int> m_itemTypes;
};

#endif // GTPYROOTBROWSERITEM_H
