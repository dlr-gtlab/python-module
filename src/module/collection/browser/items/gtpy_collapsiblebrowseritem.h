/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collapsiblebrowseritem.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.08.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCOLLAPSIBLEBROWSERITEM_H
#define GTPYCOLLAPSIBLEBROWSERITEM_H

#include "gtpy_abstractbrowseritem.h"

/**
 * @brief The GtpyCollapsibleBrowserItem class
 */
class GtpyCollapsibleBrowserItem : public GtpyAbstractBrowserItem
{
public:
    /**
     * @brief GtpyCollapsibleBrowserItem
     * @param ident
     */
    explicit GtpyCollapsibleBrowserItem(const QString& ident);

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
     * @brief Selects all children.
     */
    virtual void selectAllChildren() override;

    /**
     * @brief Unselects all children.
     */
    virtual void unselectAllChildren() override;

    /**
     * @brief Returns a list of all uncollapsible children.
     * @return A list of all uncollapsible children.
     */
    virtual QList<GtCollectionNetworkItem> uncollapsibleChilren() override;

    /**
     * @brief Returns a list of all selected items.
     * @return A list of all selected items.
     */
    virtual QList<GtCollectionNetworkItem> selectedItems() override;

    /**
     * @brief Returns the first collapsible child with the given ident.
     * @param ident Ident of the searched child.
     * @return The first collapsible child with the given ident.
     */
    GtpyCollapsibleBrowserItem* collapsibleChild(const QString& ident);

    /**
     * @brief Appends the given item to the list of child items.
     * @param item New child item.
     * @param hierarchy List of additional parents of the new child.
     */
    void appendChild(GtpyAbstractBrowserItem* item, QStringList hierarchy);

private:
    /**
     * @brief appendChild Appends the given item to the list of child items.
     * @param item New child item.
     */
    void appendChild(GtpyAbstractBrowserItem* item);

    /// Identification.
    QString m_ident;
};

#endif // GTPYCOLLAPSIBLEBROWSERITEM_H
