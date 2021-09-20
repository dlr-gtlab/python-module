/* GTlab - Gas Turbine laboratory
 * Source File: gtpylocalitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYLOCALITEM_H
#define GTPYLOCALITEM_H

#include "gt_collectionitem.h"

#include "gtpy_abstractlocalitem.h"

/**
 * @brief The GtpyLocalItem class
 */
class GtpyLocalItem : public GtpyAbstractLocalItem
{
public:
    /**
     * @brief GtpyLocalItem
     * @param item Collection item.
     */
    explicit GtpyLocalItem(const GtCollectionItem& item);

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
    virtual QString version() const override;

    /**
     * @brief Returns the description of the item.
     * @return The description of the item.
     */
    virtual QString description() const override;

    /**
     * @brief Returns the collection item.
     * @return The collection item.
     */
    GtCollectionItem item() const;

private:
    /// Collection network item.
    GtCollectionItem m_item;
};


#endif // GTPYLOCALITEM_H
