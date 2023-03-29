/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectionscontainer.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCONNECTIONSCONTAINER_H
#define GTPYCONNECTIONSCONTAINER_H

#include "gt_object.h"

class GtpyConnection;

class GtpyConnectionContainer : public GtObject
{
    Q_OBJECT

public:
    explicit GtpyConnectionContainer(GtObject* parent = nullptr);

    void addConnection(GtpyConnection* connection);

    GtpyConnection* at(int row) const;

    int count() const;

private:
    const QList<GtpyConnection*> connections() const;
};

#endif // GTPYCONNECTIONSCONTAINER_H
