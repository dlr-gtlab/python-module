/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_taskstylemodel.cpp
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QMimeData>

#include "gt_object.h"
#include "gt_processcomponent.h"
#include "gt_calculator.h"
#include "gt_calculatordata.h"
#include "gt_calculatorfactory.h"
#include "gt_extendedcalculatordata.h"
#include "gt_task.h"
#include "gt_taskdata.h"
#include "gt_taskfactory.h"
#include "gt_extendedtaskdata.h"
#include "gt_datamodel.h"
#include "gt_processfactory.h"
#if GT_VERSION >= 0x020000
#include "gt_taskgroup.h"
#else
#include "gt_processdata.h"
#endif

#include "gtpy_icons_compat.h"

#include "gtpy_taskstylemodel.h"

GtpyTaskStyleModel::GtpyTaskStyleModel(QObject* parent)
    : QIdentityProxyModel(parent)
{

}

QVariant
GtpyTaskStyleModel::data(const QModelIndex& index, int role) const
{
    // read column
    const int col = index.column();

    // switch roles
    switch (role)
    {
        case Qt::ToolTipRole:
        {
            return QVariant();
        }

        case Qt::DecorationRole:
        {
            GtObject* item = static_cast<GtObject*>(
                                 index.internalPointer());

            GtProcessComponent* pc =
                qobject_cast<GtProcessComponent*>(item);

            if (pc == Q_NULLPTR)
            {
                return QVariant();
            }

            if (col == 0)
            {
                if (qobject_cast<GtCalculator*>(pc))
                {
                    QString className = pc->metaObject()->className();
                    GtCalculatorData calcData =
                        gtCalculatorFactory->calculatorData(className);

                    GtExtendedCalculatorDataImpl* eData =
                        dynamic_cast<GtExtendedCalculatorDataImpl*>(
                            calcData.get());

                    if (eData != Q_NULLPTR)
                    {
                        return eData->icon;
                    }
                    else
                    {
                        return GTPY_ICON(calculator);
                    }
                }
                else if (qobject_cast<GtTask*>(pc))
                {
                    QString className = pc->metaObject()->className();
                    GtTaskData taskData =
                        gtTaskFactory->taskData(className);

                    GtExtendedTaskDataImpl* eData =
                        dynamic_cast<GtExtendedTaskDataImpl*>(
                            taskData.get());

                    if (eData != Q_NULLPTR)
                    {
                        return eData->icon;
                    }
                    else
                    {
                        return GTPY_ICON(process);
                    }
                }
            }
        }

        default:
            break;
    }

    QModelIndex source_index = mapToSource(index);

    if (!source_index.isValid())
    {
        return QVariant();
    }

    GtObject* item = static_cast<GtObject*>(source_index.internalPointer());

    if (item)
    {
        if (index.column() == 0)
        {
            switch (role)
            {
                case Qt::ForegroundRole:
                {
                    if (item != m_rootTask && (qobject_cast<GtTask*>(item) ||
                            item->parent() != m_rootTask))
                    {
                            return QColor(Qt::gray);
                    }

                    break;
                }
                default:
                    break;
            }
        }

    }

    return QIdentityProxyModel::data(index, role);
}

Qt::ItemFlags
GtpyTaskStyleModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    // collect default flags
    Qt::ItemFlags defaultFlags = QIdentityProxyModel::flags(index);

    // get object
    GtObject* object = static_cast<GtObject*>(index.internalPointer());

    GtCalculator* pc = qobject_cast<GtCalculator*>(object);

    if (pc && (!m_rootTask || pc->parent() == m_rootTask))
    {
        defaultFlags = defaultFlags | Qt::ItemIsEditable;
    }
    else
    {
        defaultFlags = defaultFlags & ~Qt::ItemIsEditable;
    }

    // add drop enabled flag
    defaultFlags = defaultFlags | Qt::ItemIsDropEnabled;

    // add drag enabled flag
    defaultFlags = defaultFlags | Qt::ItemIsDragEnabled;

    return defaultFlags;
}

bool
GtpyTaskStyleModel::canDropMimeData(const QMimeData* data,
                                    Qt::DropAction /*action*/, int /*row*/,
                                    int /*column*/,
                                    const QModelIndex& parent) const
{
    // recreate object from mimedata
    GtObject* obj = gtDataModel->objectFromMimeData(data, true,
                    gtProcessFactory);

    // check object
    if (obj == Q_NULLPTR)
    {
        gtError() << tr("Could not restore object from mimedata!");
        return false;
    }

    // get parent object
    GtObject* parentObj = static_cast<GtObject*>(parent.internalPointer());

    if (qobject_cast<GtCalculator*>(obj))
    {
        // handle calculator drop
        if (qobject_cast<GtTask*>(parentObj))
        {
            return true;
        }
    }
    else if (const GtTask* task = qobject_cast<GtTask*>(obj))
    {
        // handle task drop
#if GT_VERSION >= 0x020000
        if (qobject_cast<GtTaskGroup*>(parentObj))
#else
        if (qobject_cast<GtProcessData*>(parentObj))
#endif
        {
            return true;
        }
        else if (const GtTask* parentTask = qobject_cast<GtTask*>(parentObj))
        {
            if (parentTask != task)
            {
                return true;
            }
        }
    }

    return false;
}

bool
GtpyTaskStyleModel::moveRows(const QModelIndex& sourceParent,
                             int sourceRow, int count,
                             const QModelIndex& destinationParent,
                             int destinationChild)
{
    // check model index
    if (!sourceParent.isValid() || !destinationParent.isValid())
    {
        return false;
    }

    // get source parent
    GtObject* srcParentObj =
        static_cast<GtObject*>(sourceParent.internalPointer());

    // get destination parent
    GtObject* dstParentObj =
        static_cast<GtObject*>(destinationParent.internalPointer());

    // get source children
    QList<GtObject*> srcChilds = srcParentObj->findDirectChildren<GtObject*>();

    // number of source children
    const int nosc = srcChilds.size();

    // check source indices
    if (sourceRow < 0)
    {
        qWarning() << "source row < 0!";
        return false;
    }

    if (count <= 0)
    {
        qWarning() << "count <= 0!";
        return false;
    }

    if (sourceRow >= nosc || (sourceRow + count) > nosc)
    {
        qWarning() << "invalid source row!";
        return false;
    }

    // number of destination children
    const int nodc = dstParentObj->childCount<GtObject*>();

    if (destinationChild > nodc)
    {
        qWarning() << "destinationChild out of index!";
        return false;
    }

    QList<GtObject*> objsToMove;

    for (int i = sourceRow; i < (sourceRow + count); i++)
    {
        objsToMove << srcChilds[i];
    }

    if (destinationChild == -1)
    {
        // handle source == destination
        if (srcParentObj == dstParentObj)
        {
            foreach (GtObject* child, objsToMove)
            {
                child->setParent(Q_NULLPTR);
                dstParentObj->appendChild(child);
            }
        }
        else
        {
            foreach (GtObject* child, objsToMove)
            {
                dstParentObj->appendChild(child);
            }
        }
    }
    else
    {
        // handle source == destination
        if (srcParentObj == dstParentObj)
        {
            if (sourceRow == destinationChild ||
                    sourceRow == (destinationChild - 1))
            {
                return false;
            }

            if (sourceRow < destinationChild)
            {
                destinationChild = destinationChild - count;
            }

            foreach (GtObject* child, objsToMove)
            {
                child->setParent(Q_NULLPTR);
                dstParentObj->insertChild(destinationChild, child);
                destinationChild++;
            }
        }
        else
        {
            foreach (GtObject* child, objsToMove)
            {
                child->setParent(Q_NULLPTR);
                dstParentObj->insertChild(destinationChild, child);
                destinationChild++;
            }
        }
    }

    return true;
}

bool
GtpyTaskStyleModel::dropMimeData(const QMimeData* data,
                                 Qt::DropAction action,
                                 int row, int /*column*/,
                                 const QModelIndex& parent)
{
    // check action -> only move action is supported
    if (action != Qt::MoveAction)
    {
        return false;
    }

    // get destination object
    GtObject* destObj = static_cast<GtObject*>(parent.internalPointer());

    if (destObj == Q_NULLPTR)
    {
        return false;
    }

    // check mime data format
    if (!data->hasFormat(GtObject::staticMetaObject.className()))
    {
        return false;
    }

    // restore memento from mime data
    GtObjectMemento memento(data->data(GtObject::staticMetaObject.className()));

    // check memento
    if (memento.isNull())
    {
        return false;
    }

    // find object
    GtObject* obj = destObj->getObjectByUuid(memento.uuid());

    // check object
    if (obj == Q_NULLPTR)
    {
        return false;
    }

    // find parent object
    GtObject* parentObj = obj->parentObject();

    // check parent object
    if (parentObj == Q_NULLPTR)
    {
        return false;
    }

    // get source model index for object
    QModelIndex objSrcIndex = createIndex(0,0, parentObj);

    // check source index
    if (!objSrcIndex.isValid())
    {
        return false;
    }

    // map index from source model
    QModelIndex objIndex = mapFromSource(objSrcIndex);

    // check index
    if (!objIndex.isValid())
    {
        return false;
    }

    // get row of source object
    const int objRow = obj->childNumber();

    // check row
    if (objRow < 0)
    {
        return false;
    }

    beginMoveRows(objIndex, objRow, objRow, parent, row);

    if (!moveRow(objIndex, objRow, parent, row))
    {
        return false;
    }

    endMoveRows();

    return true;
}

void
GtpyTaskStyleModel::setRootTask(GtpyTask* rootTask)
{
    m_rootTask = rootTask;
}
