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

/**
 * @brief The GtpyCollapsibleLocalItem class
 */
class GtpyCollapsibleLocalItem : public GtpyAbstractLocalItem
{
public:
    /**
     * @brief GtpyCollapsibleLocalItem
     * @param ident Identification of the item.
     */
    GtpyCollapsibleLocalItem(QString ident);

    /**
     * @brief Returns whether the item is collapsible or not.
     * @return Whether the item is collapsible or not.
     */
    virtual bool isCollapsible() const override;

    /**
     * @brief Returns the identification ot the item.
     * @return The identification of the item.
     */
    virtual QString ident() const override;

    /**
     * @brief Appends the given item to the list of child items.
     * @param item New child item.
     * @param hierarchy List of additional parents of the new child.
     */
    void appendChild(GtpyAbstractLocalItem* item, QStringList hierarchy);

private:
    /**
     * @brief appendChild Appends the given item to the list of child items.
     * @param item New child item.
     */
    void appendChild(GtpyAbstractLocalItem* item);

    /**
     * @brief Returns the first collapsible child item with the given ident.
     * @param ident Ident of the searched child.
     * @return The first collapsible child item with the given ident.
     */
    GtpyCollapsibleLocalItem* collapsibleChild(const QString& ident);

    /// Identification.
    QString m_ident;
};

#endif // GTPYCOLLAPSIBLELOCALITEM_H
