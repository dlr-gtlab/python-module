/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodelitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONBROWSERMODELITEM_H
#define GTPYCOLLECTIONBROWSERMODELITEM_H

#include <QVector>

#include "gt_collectionnetworkitem.h"

class GtpyCollectionBrowserModelItem
{
public:
    GtpyCollectionBrowserModelItem(const GtCollectionNetworkItem& item);

    /**
     * @brief item
     * @return
     */
    GtCollectionNetworkItem item() const;

    /**
     * @brief isSelected
     * @return
     */
    bool isSelected() const;

    /**
     * @brief setSelected
     * @param selected
     */
    void setSelected(bool selected);

private:
    /// Collection network item.
    GtCollectionNetworkItem m_item;

    /// Item selected indicator
    bool m_selected;

};

#endif // GTPYCOLLECTIONBROWSERMODELITEM_H
