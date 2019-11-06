/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskitemmodel.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QMimeData>

#include "gt_object.h"
#include "gt_command.h"
#include "gt_application.h"
#include "gt_objectmemento.h"

#include "gtpy_taskitemmodel.h"

GtpyTaskItemModel::GtpyTaskItemModel(GtObject* root, QObject* parent) :
    QAbstractItemModel(parent), m_root(root)
{
    m_objectModel = new GtObjectModel(root, this);
}

int
GtpyTaskItemModel::columnCount(const QModelIndex& /*parent*/) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return 0;
    }

    return m_objectModel->columnCount();
}

int
GtpyTaskItemModel::rowCount(const QModelIndex& parent) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return 0;
    }

    return m_objectModel->rowCount(parent);
}

QModelIndex
GtpyTaskItemModel::index(int row, int col,
                                const QModelIndex& parent) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return QModelIndex();
    }

    return m_objectModel->index(row, col, parent);
}

QModelIndex
GtpyTaskItemModel::parent(const QModelIndex& index) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return QModelIndex();
    }

    return m_objectModel->parent(index);
}

QVariant
GtpyTaskItemModel::data(const QModelIndex& index, int role) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return QVariant();
    }

    return m_objectModel->data(index, role);
}

GtObject*
GtpyTaskItemModel::objectFromIndex(const QModelIndex& index) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return Q_NULLPTR;
    }

    return m_objectModel->objectFromIndex(index);
}

QModelIndex
GtpyTaskItemModel::indexFromObject(GtObject* obj) const
{
    if (m_objectModel == Q_NULLPTR)
    {
        return QModelIndex();
    }

    return m_objectModel->indexFromObject(obj);
}

bool
GtpyTaskItemModel::setData(const QModelIndex& index,
                                  const QVariant& value, int role)
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
    if (item == Q_NULLPTR)
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
GtpyTaskItemModel::mimeTypes() const
{
    QStringList types;
    types << GtObject::staticMetaObject.className();
    return types;
}

QMimeData*
GtpyTaskItemModel::mimeData(const QModelIndexList& indexes) const
{
    if (indexes.size() > 0)
    {
        QModelIndex index = indexes.value(0);

        if (index.isValid())
        {
            GtObject* obj = objectFromIndex(index);

            if (obj == Q_NULLPTR)
            {
                return new QMimeData();
            }

            // create memento
            GtObjectMemento memento = obj->toMemento(true);

            QMimeData* mimeData = new QMimeData;

            // append memento to mime data
            mimeData->setData(GtObject::staticMetaObject.className(),
                              memento.toByteArray());

            return mimeData;
        }
    }

    return new QMimeData();
}

void
GtpyTaskItemModel::updateView()
{
    beginResetModel();
    endResetModel();
}

Qt::DropActions
GtpyTaskItemModel::supportedDropActions() const
{
     return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions
GtpyTaskItemModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags
GtpyTaskItemModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    GtObject* object = objectFromIndex(index);

    if (object != Q_NULLPTR)
    {
        defaultFlags = defaultFlags | Qt::ItemIsDropEnabled;
    }

    return Qt::ItemIsDragEnabled | defaultFlags;
}
