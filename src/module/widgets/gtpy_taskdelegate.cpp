/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskdelegate.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QLineEdit>

#include "gt_globals.h"
#include "gt_regexp.h"

#include "gtpy_taskdelegate.h"

GtpyTaskDelegate::GtpyTaskDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{

}

QWidget*
GtpyTaskDelegate::createEditor(QWidget* parent,
                                    const QStyleOptionViewItem& /*option*/,
                                    const QModelIndex& /*index*/) const
{
    QLineEdit* lineEdit = new QLineEdit(parent);

#if GT_VERSION < 0x020000
    QRegExp onlyLettersAndNumbersAndSpaceExp = GtRegExp::onlyLettersAndNumbersAndSpace();
#else
    static QRegularExpression onlyLettersAndNumbersAndSpaceExp("[A-Za-z0-9\\_\\-\\[\\]\\s\\␣]+");
#endif

    QValidator* validator = new QRegularExpressionValidator(onlyLettersAndNumbersAndSpaceExp, this->parent());;

    lineEdit->setValidator(validator);

    return lineEdit;
}

void GtpyTaskDelegate::setEditorData(QWidget* editor,
                                          const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return;
    }

    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    QString val = index.data(Qt::DisplayRole).toString();
    lineEdit->setText(val);
}
