/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsersortmodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 20.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_collectionbrowsersortmodel.h"

GtpyCollectionBrowserSortModel::GtpyCollectionBrowserSortModel(
    QObject* parent) : QSortFilterProxyModel(parent)
{

}

bool
GtpyCollectionBrowserSortModel::lessThan(const QModelIndex& source_left,
        const QModelIndex& source_right) const
{
    if (!source_left.parent().isValid())
    {
        return false;
    }

    return QSortFilterProxyModel::lessThan(source_left, source_right);
}
