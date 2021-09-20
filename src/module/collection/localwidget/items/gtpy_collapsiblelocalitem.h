/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collapsiblelocalitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLAPSIBLELOCALITEM_H
#define GTPYCOLLAPSIBLELOCALITEM_H

#include "gtpy_abstractlocalitem.h"

class GtpyCollapsibleLocalItem : public GtpyAbstractLocalItem
{
public:
    GtpyCollapsibleLocalItem(QString ident);

    virtual ~GtpyCollapsibleLocalItem();

    virtual bool isCollapsible() const override;

    virtual QString ident() const override;

    void appendChild(GtpyAbstractLocalItem* item, QStringList hierarchy);

private:
    void appendChild(GtpyAbstractLocalItem* item);

    GtpyCollapsibleLocalItem* collapsibleChild(const QString& ident);

    QString m_ident;

};

#endif // GTPYCOLLAPSIBLELOCALITEM_H
