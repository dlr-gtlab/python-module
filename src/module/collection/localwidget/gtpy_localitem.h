/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_localitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 25.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYLOCALITEM_H
#define GTPYLOCALITEM_H

#include "gt_collectionitem.h"

#include "gtpy_abstractlocalitem.h"

class GtpyLocalItem : public GtpyAbstractLocalItem
{
public:
    GtpyLocalItem(const GtCollectionItem& item);

    virtual ~GtpyLocalItem();

    virtual bool isCollapsible() const override;

    virtual QString ident() const override;

    virtual QString version() const override;

    virtual QString description() const override;

    GtCollectionItem item() const;

private:
    /// Collection network item.
    GtCollectionItem m_item;
};

#endif // GTPYLOCALITEM_H
