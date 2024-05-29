/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_completermodel.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (AT-TW)
 */

#include "gtpy_completermodel.h"

GtpyCompleterModel::GtpyCompleterModel(QObject* parent) :
    QAbstractListModel(parent)
{

}

int
GtpyCompleterModel::rowCount(const QModelIndex&) const
{
    return m_found.size();
}

QVariant
GtpyCompleterModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    const int row = index.row();

    if (row < 0 || row >= m_found.size())
    {
        return QVariant();
    }

    switch (role)
    {
        case Qt::DisplayRole:
            return m_found.values().at(row).name;

        default:
            break;
    }

    return QVariant();
}

void
GtpyCompleterModel::setFound(const QMap<QString, GtpyFunction>& found)
{
    m_found = found;

    beginInsertRows(QModelIndex(), m_found.size(), m_found.size());
    endInsertRows();
}

QString
GtpyCompleterModel::completion(const QModelIndex& index)
{
    const int row = index.row();

    if (row < 0 || row >= m_found.size())
    {
        return QString();
    }

    return m_found.values().at(row).completion;
}

QString
GtpyCompleterModel::completionName(const QModelIndex& index)
{
    const int row = index.row();

    if (row < 0 || row >= m_found.size())
    {
        return QString();
    }

    return m_found.values().at(row).name;
}

int
GtpyCompleterModel::completionCursorOffset(const QModelIndex& index)
{
    const int row = index.row();

    if (row < 0 || row >= m_found.size())
    {
        return 0;
    }

    int cursorOffset = m_found.values().at(row).cursorOffset;

    if (cursorOffset > completion(index).size() || cursorOffset < 0)
    {
        return 0;
    }

    return cursorOffset;
}

QString
GtpyCompleterModel::toolTipText(const QModelIndex& index)
{
    const int row = index.row();

    if (row < 0 || row >= m_found.size())
    {
        return QString();
    }

    return m_found.values().at(row).toolTip;
}
