/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskdelegate.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QLineEdit>

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

    QRegExp regExp = GtRegExp::onlyLettersAndNumbersAndSpace();

    QValidator* validator = new QRegExpValidator(regExp, this->parent());;

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
