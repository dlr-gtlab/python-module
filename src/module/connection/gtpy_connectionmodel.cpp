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

GtpyConnectionModel::GtpyConnectionModel(QObject* parent)
    : QAbstractItemModel(parent)
{

}

QVariant
GtpyConnectionModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    GtpyConnection* connection = connectionFromIndex(index);

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

    GtpyConnection* connection = m_connections->at(row);

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
    if (!m_connections)
        return 0;

    return m_connections->count();
}

int
GtpyConnectionModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 1;
}

void
GtpyConnectionModel::setConnectionContainer(GtpyConnectionContainer* container)
{
    m_connections = container;
}

GtpyConnection*
GtpyConnectionModel::connectionFromIndex(const QModelIndex& index) const
{
    if (!index.isValid())
        return nullptr;

    return static_cast<GtpyConnection*>(index.internalPointer());
}
