/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsersortmodel.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 20.09.2021
 * Author: Marvin Noethen (DLR AT-TWK)
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
