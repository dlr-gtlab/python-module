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

/**
 * @brief The GtpyRootBrowserItem class
 */
class GtpyRootBrowserItem : public GtpyAbstractBrowserItem
{
public:
    /**
     * @brief GtpyRootBrowserItem
     */
    GtpyRootBrowserItem();

    /**
     * @brief ~GtpyRootBrowserItem
     */
    virtual ~GtpyRootBrowserItem();

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
     * @brief Creates a new collapsible child item with the given type id and
     * the given ident.
     * @param typeId Type id of the new child.
     * @param ident Ident of the new child.
     * @return Returns the new collapsible child item.
     */
    GtpyCollapsibleBrowserItem* createChild(int typeId, const QString& ident);

    /**
     * @brief Returns the child with the given type id.
     * @param typeId Type id of the searched child.
     * @return The child with the given type id.
     */
    GtpyCollapsibleBrowserItem* child(int typeId);

    /**
     * @brief Deletes all child items.
     */
    void clearRoot();

private:
    /// List of item types.
    QList<int> m_itemTypes;
};

#endif // GTPYROOTBROWSERITEM_H