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

    QVariant data(const QModelIndex& index, int role) const override;

    QModelIndex index(int row, int col,
                      const QModelIndex& parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void setConnectionContainer(GtpyConnectionContainer* container);

private:
    QPointer<GtpyConnectionContainer> m_connections{};

    GtpyConnection* connectionFromIndex(const QModelIndex& index) const;
};

#endif // GTPYCONNECTIONMODEL_H
