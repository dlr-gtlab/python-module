/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_objectmodel.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 20.12.2022
 * Author: Marvin Noethen (AT-TW)
 */

#include <QMimeData>

#include "gt_object.h"
#include "gt_objectmemento.h"

#include "gtpy_objectmodel.h"

GtpyObjectModel::GtpyObjectModel(QObject* parent) : GtObjectModel(parent)
{

}

GtpyObjectModel::GtpyObjectModel(GtObject* root, QObject* parent) :
    GtObjectModel(root, parent)
{

}

bool
GtpyObjectModel::setData(const QModelIndex& index, const QVariant& value,
                         int role)
{
    // check index
    if (!index.isValid())
    {
        return false;
    }

    // check column
    if (index.column() != 0)
    {
        return false;
    }

    // check role
    if (role != Qt::EditRole)
    {
        return false;
    }

    // get object
    GtObject* item = objectFromIndex(index);

    // check object
    if (!item)
    {
        return false;
    }

    // check renameable flag
    if (!item->isRenamable())
    {
        return false;
    }

    if (value.toString() == item->objectName())
    {
        return false;
    }

    emit processComponentRenamed(item->metaObject()->className(),
                                 item->objectName(), value.toString());

    item->setObjectName(value.toString());

    return true;
}

QStringList
GtpyObjectModel::mimeTypes() const
{
    QStringList types;
    types << GtObject::staticMetaObject.className();
    return types;
}

QMimeData*
GtpyObjectModel::mimeData(const QModelIndexList& indexes) const
{
    if (!indexes.empty())
    {
        QModelIndex index = indexes.value(0);

        if (index.isValid())
        {
            GtObject* obj = objectFromIndex(index);

            return mimeDataFromObject(obj);
        }
    }

    return nullptr;
}

Qt::DropActions
GtpyObjectModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

QMimeData*
GtpyObjectModel::mimeDataFromObject(GtObject* obj, bool newUuid) const
{
    if (!obj)
    {
        return nullptr;
    }

    // create memento
    GtObjectMemento memento = obj->toMemento(!newUuid);

    QMimeData* mimeData = new QMimeData;

    // append memento to mime data
    mimeData->setData(GtObject::staticMetaObject.className(),
                      memento.toByteArray());

    // return mime data
    return mimeData;
}

void
GtpyObjectModel::update()
{
    beginResetModel();
    endResetModel();
}

