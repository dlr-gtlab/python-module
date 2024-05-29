/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_completermodel.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_COMPLETERMODEL_H
#define GTPY_COMPLETERMODEL_H

#include <QAbstractItemModel>

#include "gtpy_contextmanager.h"

/**
 * @brief The GtpyCompleterModel class
 */
class GtpyCompleterModel: public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * @brief GtPythonCompleterModel
     * @param parent Parent of the model
     */
    explicit GtpyCompleterModel(QObject* parent);

    /**
     * @brief Retruns the number of rows.
     * @param parent
     * @return The number of rows.
     */
    int rowCount(const QModelIndex& /*parent*/) const Q_DECL_OVERRIDE;

    /**
     * @brief Returns data for the given index and role.
     * @param index Index of data.
     * @param role Role that is considered.
     * @return Data for the given index and role.
     */
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

    /**
     * @brief Sets the data to the model.
     * @param found Found completions.
     */
    void setFound(const QMap<QString, GtpyFunction>& found);

    /**
     * @brief Returns the completion for the given index.
     * @param index Index of the completion
     * @return The completion for the given index.
     */
    QString completion(const QModelIndex& index);

    /**
     * @brief Returns the name of completion for the given index.
     * @param index Index of the completion
     * @return The name of completion for the given index.
     */
    QString completionName(const QModelIndex& index);

    /**
     * @brief Returns the cursor offset for the given index.
     * @param index Index of the completion
     * @return The cursor offset for the completiona at given index.
     */
    int completionCursorOffset(const QModelIndex& index);

    /**
     * @brief Returns the tooltip text for the given index.
     * @param index Index of the element whose tooltip text was queried.
     * @return The tooltip text for the given index.
     */
    QString toolTipText(const QModelIndex& index);

private:
    /// Found Completions
    QMap<QString, GtpyFunction> m_found;
};

#endif // GTPY_COMPLETERMODEL_H
