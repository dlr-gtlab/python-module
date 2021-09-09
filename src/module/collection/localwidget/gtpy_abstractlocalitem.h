/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractlocalitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 25.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYABSTRACTLOCALITEM_H
#define GTPYABSTRACTLOCALITEM_H

#include <QVector>
#include <QString>

#include "gt_collectionitem.h"

class GtpyAbstractLocalItem
{
public:
    GtpyAbstractLocalItem();

    virtual ~GtpyAbstractLocalItem();

    virtual bool isCollapsible() const = 0;

    virtual QString ident() const = 0;

    virtual QString version() const;

    virtual QString installedVersion() const;

    virtual QString description() const;

    GtCollectionItem item();

    int childCount() const;

    GtpyAbstractLocalItem* child(int row);

    void deleteChildren();

    int row() const;

    GtpyAbstractLocalItem* parentItem() const;

    void setParentItem(GtpyAbstractLocalItem* parent);

protected:
    QVector<GtpyAbstractLocalItem*> m_childItems;

private:
    GtpyAbstractLocalItem* m_parentItem;
};

#endif // GTPYABSTRACTLOCALITEM_H
