/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectionscontainer.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_connection.h"

#include "gtpy_connectioncontainer.h"

GtpyConnectionContainer::GtpyConnectionContainer(GtObject* parent)
    : GtObject(parent)
{
    setObjectName("Python connections");
}

void
GtpyConnectionContainer::addConnection(GtpyConnection* connection)
{
    this->appendChild(connection);
}

GtpyConnection*
GtpyConnectionContainer::at(int row) const
{
    if (row < 0)
    {
        return {};
    }

    QList<GtpyConnection*> list = this->connections();

    if (row >= list.size())
    {
        return {};
    }

    return list[row];
}

int
GtpyConnectionContainer::count() const
{
    return this->connections().size();
}

const QList<GtpyConnection*>
GtpyConnectionContainer::connections() const
{
    return this->findDirectChildren<GtpyConnection*>();
}
