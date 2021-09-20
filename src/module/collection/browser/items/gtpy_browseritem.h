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

/**
 * @brief The GtpyBrowserItem class
 */
class GtpyBrowserItem : public GtpyAbstractBrowserItem
{
public:
    /**
     * @brief GtpyBrowserItem
     * @param item Wrapped item.
     */
    explicit GtpyBrowserItem(const GtCollectionNetworkItem& item);

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
     * @brief Returns the version of the item.
     * @return The version of the item.
     */
    virtual QString version() const;

    /**
     * @brief Returns the installed version of the item.
     * @return The installed version of the item.
     */
    virtual QString installedVersion() const;

    /**
     * @brief Returns the collection item.
     * @return The collection item.
     */
    virtual GtCollectionNetworkItem item() const override;

private:
    /// Collection network item.
    GtCollectionNetworkItem m_item;
};

#endif // GTPYBROWSERITEM_H
