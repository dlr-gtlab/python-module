/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskdelegate.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPY_TASKDELEGATE_H
#define GTPY_TASKDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>

/**
 * @brief The GtpyTaskDelegate class
 */
class GtpyTaskDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    /**
     * @brief GtpyTaskDelegate
     * @param parent Parent object.
     */
    explicit GtpyTaskDelegate(QObject* parent);

    /**
     * @brief Creates a line editor.
     * @param parent Parent of line editor.
     * @param option Options.
     * @param index Index.
     * @return Created line editor.
     */
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex&index) const Q_DECL_OVERRIDE;

    /**
     * @brief Sets value from index to editor.
     * @param editor Line editor.
     * @param index Index of value.
     */
    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const Q_DECL_OVERRIDE;
};

#endif // GTPY_TASKDELEGATE_H
