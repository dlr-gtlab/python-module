/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionlocalmodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QIcon>
#include <QDir>

#include "gt_application.h"

#include "gtpy_contextmanager.h"
#include "gtpy_globals.h"
#include "gtpy_collapsiblelocalitem.h"
#include "gtpy_localitem.h"

#include "gtpy_collectionlocalmodel.h"

GtpyCollectionLocalModel::GtpyCollectionLocalModel(QObject* parent) :
    QAbstractItemModel(parent), m_showInfoColumns(true), m_rootItem(Q_NULLPTR)
{
    m_rootItem = new GtpyCollapsibleLocalItem("Root");
}

GtpyCollectionLocalModel::~GtpyCollectionLocalModel()
{
    if (m_rootItem)
    {
        delete m_rootItem;
    }
}

int
GtpyCollectionLocalModel::rowCount(const QModelIndex& parent) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return 0;
    }

    GtpyAbstractLocalItem* parentItem;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem =
            static_cast<GtpyAbstractLocalItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int
GtpyCollectionLocalModel::columnCount(const QModelIndex& /*parent*/) const
{
    if (m_showInfoColumns)
    {
        return 3;
    }
    else
    {
        return 1;
    }
}

QVariant
GtpyCollectionLocalModel::data(const QModelIndex& index, int role) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return QVariant();
    }

    if (!index.isValid())
    {
        return QVariant();
    }

    const int col = index.column();

    GtpyAbstractLocalItem* item = static_cast<GtpyAbstractLocalItem*>
                                  (index.internalPointer());

    if (!item)
    {
        return QVariant();
    }

    bool dark = false;
#if GT_VERSION >= 0x020000
    dark = gtApp->inDarkMode();
#endif

    if (item->isCollapsible())
    {
        if (role == Qt::DisplayRole && col == 0)
        {
            return item->ident();
        }
        else if (item->parentItem() == m_rootItem)
        {
            if (role == Qt::BackgroundRole)
            {
                if (dark)
                {
                     return QColor(45, 45, 45);
                }
                else
                {
                    return QColor(245, 245, 245);
                }
            }
        }
        else
        {
            if (role == Qt::BackgroundRole)
            {
                if (dark)
                {
                     return QColor(50, 50, 50);
                }
                else
                {
                    return QColor(250, 250, 250);
                }
            }
        }
    }
    else
    {
        switch (role)
        {
            case Qt::DisplayRole:
                if (col == 0)
                {
                    return item->ident();
                }
                else if (col == 2)
                {
                    return item->version();
                }

                break;

            case Qt::ToolTipRole:
                return item->description();
                break;

            case Qt::DecorationRole:
                if (col == 0)
                {
                    return gtApp->icon(QStringLiteral("propertyIcon_16.png"));
                }
                else if (col == 1)
                {
                    return gtApp->icon(QStringLiteral("infoBlueIcon_16.png"));
                }

                break;

            default:
                break;
        }
    }

    return QVariant();
}

QVariant
GtpyCollectionLocalModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (orientation != Qt::Horizontal)
    {
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    switch (role)
    {
        case Qt::DisplayRole:
            if (section == 0)
            {
                return tr("Name");
            }
            else if (section == 1)
            {
                return QString();
            }
            else if (section == 2)
            {
                return tr("Version");
            }

            break;

        default:
            break;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void
GtpyCollectionLocalModel::setItems(const QList<GtCollectionItem>& items)
{
    beginResetModel();

    m_rootItem->deleteChildren();

    foreach (GtCollectionItem item, items)
    {
        QStringList hierarchy;

        QString cat = item.property(GtpyGlobals::COLLECTION_cat).toString();

        if (!cat.isEmpty())
        {
            hierarchy << cat;

            cat = item.property(GtpyGlobals::COLLECTION_subcat).toString();

            if (!cat.isEmpty())
            {
                hierarchy << cat;
            }
        }

        m_rootItem->appendChild(new GtpyLocalItem(item), hierarchy);
    }

    endResetModel();
}

QModelIndex
GtpyCollectionLocalModel::index(int row, int column,
                                const QModelIndex& parent) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return QModelIndex();
    }

    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    GtpyAbstractLocalItem* parentItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<GtpyAbstractLocalItem*>(
                         parent.internalPointer());
    }

    GtpyAbstractLocalItem* childItem = parentItem->child(row);

    if (childItem)
    {
        if (childItem->childCount() > 0 || !childItem->isCollapsible())
        {
            return createIndex(row, column, childItem);
        }
    }

    return QModelIndex();
}

QModelIndex
GtpyCollectionLocalModel::parent(const QModelIndex& index) const
{
    if (m_rootItem == Q_NULLPTR)
    {
        return QModelIndex();
    }

    if (!index.isValid())
    {
        return QModelIndex();
    }

    GtpyAbstractLocalItem* childItem =
        static_cast<GtpyAbstractLocalItem*>(index.internalPointer());

    GtpyAbstractLocalItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

GtCollectionItem
GtpyCollectionLocalModel::itemFromIndex(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return GtCollectionItem();
    }

    if (index.model() != this)
    {
        return GtCollectionItem();
    }

    QModelIndex parent = index.parent();

    if (!parent.isValid())
    {
        return GtCollectionItem();
    }

    GtpyLocalItem* item = static_cast<GtpyLocalItem*>
                          (index.internalPointer());

    if (!item)
    {
        return GtCollectionItem();
    }

    if (item->isCollapsible())
    {
        return GtCollectionItem();
    }

    return item->item();
}

bool
GtpyCollectionLocalModel::uninstallItem(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return false;
    }

    if (index.model() != this)
    {
        return false;
    }

    GtpyLocalItem* item =
        static_cast<GtpyLocalItem*>(index.internalPointer());

    if (!item)
    {
        return false;
    }

    GtpyCollapsibleLocalItem* parentItem =
        static_cast<GtpyCollapsibleLocalItem*>(item->parentItem());

    if (!parentItem)
    {
        return false;
    }

    const int row = index.row();

    if (row < 0 || row >= parentItem->childCount())
    {
        return false;
    }

    QModelIndex parentIndex = index.parent();

    beginRemoveRows(parentIndex, row, row);

    QString uuid = item->item().uuid();

    bool success = false;

    if (parentItem->deleteChild(row))
    {
        item = Q_NULLPTR;
        QString collectionPath = GtpyContextManager::collectionPath();

        if (!collectionPath.isEmpty())
        {
            if (!uuid.isEmpty())
            {
                QDir dir(collectionPath + QDir::separator() + uuid);

                if (dir.exists())
                {
                    success = dir.removeRecursively();
                }
            }
        }
    }

    endRemoveRows();

    if (success)
    {
        int childCount = parentItem->childCount();

        while (childCount == 0)
        {
            int parentRow = parentItem->row();

            parentItem = static_cast<GtpyCollapsibleLocalItem*>(
                             parentItem->parentItem());

            if (parentItem)
            {
                parentIndex = parentIndex.parent();

                beginRemoveRows(parentIndex, parentRow, parentRow);

                parentItem->deleteChild(parentRow);

                if (parentItem == m_rootItem)
                {
                    childCount = -1;
                }
                else
                {
                    childCount = parentItem->childCount();
                }

                endRemoveRows();
            }
            else
            {
                childCount = -1;
            }

        }
    }

    return success;
}

void
GtpyCollectionLocalModel::setShowInfoColumns(bool val)
{
    if (m_showInfoColumns == val)
    {
        return;
    }

    beginResetModel();
    m_showInfoColumns = val;
    endResetModel();
}

