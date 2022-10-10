/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectionmodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_connection.h"

#include "gtpy_connectionmodel.h"

GtpyConnectionModel::GtpyConnectionModel(GtpyConnectionContainer* connections,
                                         QObject* parent)
    : QAbstractItemModel(parent),
      m_connections{connections}
{

}

QVariant
GtpyConnectionModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    auto* connection = connectionFromIndex(index);

    if (!connection)
        return QVariant();

    return connection->objectName();
}

QModelIndex
GtpyConnectionModel::index(int row, int col, const QModelIndex& parent) const
{
    if (!hasIndex(row, col, parent))
        return {};

    if (parent.isValid())
        return {};

    if (!m_connections)
        return {};

    auto* connection = m_connections->at(row);

    if (!connection)
    {
        return {};
    }

    return createIndex(row, col, connection);
}

QModelIndex
GtpyConnectionModel::parent(const QModelIndex& /*index*/) const
{
    return {};
}

int
GtpyConnectionModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_connections ? m_connections->count() : 0;
}

int
GtpyConnectionModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

void
GtpyConnectionModel::addConnection(GtpyConnection* connection)
{
    if (!m_connections)
        return ;

    if (!connection)
        return;

    int count = m_connections->count();
    beginInsertRows({}, count + 1, count + 1);
    m_connections->addConnection(connection);
    endInsertRows();
}

GtpyConnection*
GtpyConnectionModel::connectionFromIndex(const QModelIndex& index) const
{
    return index.isValid() ?
                static_cast<GtpyConnection*>(index.internalPointer()) : nullptr;
}
