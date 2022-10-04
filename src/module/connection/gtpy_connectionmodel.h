/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_connectionmodel.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 04.10.2022
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCONNECTIONMODEL_H
#define GTPYCONNECTIONMODEL_H

#include <QPointer>
#include <QAbstractItemModel>

#include "gtpy_connectioncontainer.h"

class GtpyConnectionModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GtpyConnectionModel(QObject* parent = nullptr);

private:
    QPointer<GtpyConnectionContainer> m_connections{};
};

#endif // GTPYCONNECTIONMODEL_H
