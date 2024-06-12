/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractbrowseritem.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 22.08.2021
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYABSTRACTBROWSERITEM_H
#define GTPYABSTRACTBROWSERITEM_H

#include <QVector>
#include <QString>

#include "gt_collectionnetworkitem.h"

/**
 * @brief The GtpyAbstractBrowserItem class
 */
class GtpyAbstractBrowserItem
{
public:
    /**
     * @brief GtpyAbstractBrowserItem
     */
    GtpyAbstractBrowserItem();

    /**
     * @brief ~GtpyAbstractBrowserItem
     */
    virtual ~GtpyAbstractBrowserItem();

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
    virtual GtCollectionNetworkItem item() const;

    /**
     * @brief Selects all children.
     */
    virtual void selectAllChildren();

    /**
     * @brief Unselects all children.
     */
    virtual void unselectAllChildren();

    /**
     * @brief Returns a list of all selected items.
     * @return A list of all selected items.
     */
    virtual QList<GtCollectionNetworkItem> selectedItems();

    /**
     * @brief Returns a list of all uncollapsible children.
     * @return A list of all uncollapsible children.
     */
    virtual QList<GtCollectionNetworkItem> uncollapsibleChilren();

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
    GtpyAbstractBrowserItem* child(int row);

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
    GtpyAbstractBrowserItem* parentItem() const;

    /**
     * @brief Sets the parent item to the given item.
     * @param parent The new parent item.
     */
    void setParentItem(GtpyAbstractBrowserItem* parent);

    /**
     * @brief Returns whether the item is selected or not.
     * @return Whether the item is selected or not.
     */
    bool isSelected() const;

    /**
     * @brief Set item selected.
     * @param selected
     */
    void setSelected(bool selected);

    /**
     * @brief Set item type id.
     * @param typeId Type id.
     */
    void setTypeId(int typeId);

    /**
     * @brief Retruns the item type id.
     * @return The item type id.
     */
    int typeId() const;

protected:
    /// Child items.
    QVector<GtpyAbstractBrowserItem*> m_childItems;

private:
    /// Parent item.
    GtpyAbstractBrowserItem* m_parentItem;

    /// Item selected indicator.
    bool m_selected;

    /// Type id.
    int m_typeId;
};

#endif // GTPYABSTRACTBROWSERITEM_H
