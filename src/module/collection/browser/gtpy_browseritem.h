/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_browseritem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYBROWSERITEM_H
#define GTPYBROWSERITEM_H

#include "gt_collectionnetworkitem.h"

#include "gtpy_abstractbrowseritem.h"

class GtpyBrowserItem : public GtpyAbstractBrowserItem
{
public:

    GtpyBrowserItem(const GtCollectionNetworkItem& item);

    virtual bool isCollapsible() const override;

    virtual QString ident() const override;

    virtual QString version() const override;

    virtual QString installedVersion() const override;

    virtual QString description() const override;

    GtCollectionNetworkItem item() const;

private:
    /// Collection network item.
    GtCollectionNetworkItem m_item;
};

#endif // GTPYBROWSERITEM_H
