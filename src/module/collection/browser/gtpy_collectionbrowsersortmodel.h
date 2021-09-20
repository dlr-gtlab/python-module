/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsersortmodel.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONBROWSERSORTMODEL_H
#define GTPYCOLLECTIONBROWSERSORTMODEL_H

#include <QSortFilterProxyModel>

#include "gtpy_collectionbrowsermodel.h"

/**
 * @brief The GtpyCollectionBrowserSortModel class
 */
class GtpyCollectionBrowserSortModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionBrowserSortModel
     * @param parent Parent object.
     */
    GtpyCollectionBrowserSortModel(QObject* parent = Q_NULLPTR);

protected:
    /**
     * @brief Reimplemented to exclude main ("Available", "Update available!",
     * "Installed") from sorting.
     * @param source_left Index of the left item.
     * @param source_right Index of the right item.
     * @return True if left item is less than right item.
     */
    virtual bool lessThan(const QModelIndex& source_left,
                          const QModelIndex& source_right) const override;
};

#endif // GTPYCOLLECTIONBROWSERSORTMODEL_H
