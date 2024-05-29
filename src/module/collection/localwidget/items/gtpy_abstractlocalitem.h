/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractlocalitem.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 16.09.2021
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPYABSTRACTLOCALITEM_H
#define GTPYABSTRACTLOCALITEM_H

#include <QVector>
#include <QString>

#include "gt_collectionitem.h"

/**
 * @brief The GtpyAbstractLocalItem class
 */
class GtpyAbstractLocalItem
{
public:
    /**
     * @brief GtpyAbstractLocalItem
     */
    GtpyAbstractLocalItem();

    /**
     * @brief ~GtpyAbstractLocalItem
     */
    virtual ~GtpyAbstractLocalItem();

    /**
     * @brief Returns whether the item is collapsible or not.
     * @return Whether the item is collapsible or not.
     */
    virtual bool isCollapsible() const = 0;

    /**
     * @brief Returns the identification ot the item.
     * @return The identification of the item.
     */
    virtual QString ident() const = 0;

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
     * @brief Returns the description of the item.
     * @return The description of the item.
     */
    virtual QString description() const;

    /**
     * @brief Returns the collection item.
     * @return The collection item.
     */
    GtCollectionItem item() const;

    /**
     * @brief Returns the number of child items.
     * @return Number of child items.
     */
    int childCount() const;

    /**
     * @brief Retruns the child at the given row.
     * @param row Row of the child to be returned.
     * @return The child at the given row or a null pointer if the row is not
     * valid.
     */
    GtpyAbstractLocalItem* child(int row);

    /**
     * @brief Deletes the child at the given row.
     * @param row Row ot the child to be deleted.
     * @return True, if the deletion was successful
     */
    bool deleteChild(int row);

    /**
     * @brief Deletes all children.
     */
    void deleteChildren();

    /**
     * @brief Returns the row of the item.
     * @return The row of the item.
     */
    int row() const;

    /**
     * @brief Returns the parent item of the item.
     * @return The parent item of the item.
     */
    GtpyAbstractLocalItem* parentItem() const;

    /**
     * @brief Sets the parent item to the given item.
     * @param parent The new parent item.
     */
    void setParentItem(GtpyAbstractLocalItem* parent);

protected:
    /// Children.
    QVector<GtpyAbstractLocalItem*> m_childItems;

private:
    /// Parent item.
    GtpyAbstractLocalItem* m_parentItem;
};

#endif // GTPYABSTRACTLOCALITEM_H
