/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QIcon>

#include "gt_application.h"

#include "gtpy_globals.h"
#include "gtpy_icons_compat.h"
#include "gtpy_browseritem.h"
#include "gtpy_collapsiblebrowseritem.h"
#include "gtpy_rootbrowseritem.h"

#include "gtpy_collectionbrowsermodel.h"

GtpyCollectionBrowserModel::GtpyCollectionBrowserModel(QObject* parent) :
    QAbstractItemModel(parent), m_rootItem(nullptr)
{
    m_rootItem = new GtpyRootBrowserItem();
}

GtpyCollectionBrowserModel::~GtpyCollectionBrowserModel()
{
    if (m_rootItem)
    {
        delete m_rootItem;
    }
}

int
GtpyCollectionBrowserModel::rowCount(const QModelIndex& parent) const
{
    if (!m_rootItem)
    {
        return 0;
    }

    GtpyAbstractBrowserItem* parentItem;

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
            static_cast<GtpyAbstractBrowserItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int
GtpyCollectionBrowserModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 4;
}

QVariant
GtpyCollectionBrowserModel::data(const QModelIndex& index, int role) const
{
    if (!m_rootItem)
    {
        return {};
    }

    if (!index.isValid())
    {
        return {};
    }

    const int col = index.column();

    GtpyAbstractBrowserItem* item = static_cast<GtpyAbstractBrowserItem*>
                                    (index.internalPointer());

    if (!item)
    {
        return {};
    }

    QColor main(245, 245, 245);
    QColor darker(240, 240, 240);
    QColor bright1(250, 250, 250);
    QColor bright2(255, 255, 255);

    bool dark = false;
#if GT_VERSION >= 0x020000
    dark = gtApp->inDarkMode();
#endif

    if (dark)
    {
        main.setRgb(45, 45, 45);
        darker.setRgb(40, 40, 40);
        bright1.setRgb(50, 50, 50);
        bright2.setRgb(55, 55, 55);
    }

    if (item->isCollapsible())
    {
        switch (role)
        {
            case Qt::DisplayRole:
                if (col == 0)
                {
                    return item->ident();
                }

                break;


            case Qt::DecorationRole:
                if (col == 0)
                {
                    if (item == m_rootItem->child(UpdateAvailableItem))
                    {
                        return GTPY_ICON(update);
                    }
                    else if (item == m_rootItem->child(AvailableItem))
                    {
                        return GTPY_ICON(layers);
                    }
                    else if (item == m_rootItem->child(InstalledItem))
                    {
                        return GTPY_ICON(collection);
                    }
                }

                break;


            case Qt::BackgroundRole:
                if (item == m_rootItem->child(UpdateAvailableItem))
                {
                    return QColor(180, 229, 190);
                }
                else if (item == m_rootItem->child(AvailableItem))
                {
                    return bright2;
                }
                else if (item == m_rootItem->child(InstalledItem))
                {
                    return darker;
                }
                else if (item->parentItem() ==
                         m_rootItem->child(UpdateAvailableItem))
                {
                    return main;
                }
                else if (item->parentItem() == m_rootItem->child(AvailableItem))
                {
                    return main;
                }
                else if (item->parentItem() == m_rootItem->child(InstalledItem))
                {
                    return main;
                }
                else
                {
                    return bright1;
                }

            default:
                break;
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
                else if (col == 3)
                {
                    GtpyCollectionItemType type =
                        static_cast<GtpyCollectionItemType>(item->typeId());

                    if (type == UpdateAvailableItem || type == InstalledItem)
                    {
                        return item->installedVersion();
                    }
                }

                break;

            case Qt::DecorationRole:
                if (col == 0)
                {
                    return GTPY_ICON(plugin);
                }
                else if (col == 1)
                {
                    return GTPY_ICON(info2);
                }

                break;

            case Qt::CheckStateRole:
                if (col == 0)
                {
                    GtpyCollectionItemType type =
                        static_cast<GtpyCollectionItemType>(item->typeId());

                    if (type == UpdateAvailableItem || type == AvailableItem)
                    {
                        if (item->isSelected())
                        {
                            return Qt::Checked;
                        }
                        else
                        {
                            return Qt::Unchecked;
                        }
                    }
                }

                break;

            case Qt::ForegroundRole:
            {
                GtpyCollectionItemType type =
                    static_cast<GtpyCollectionItemType>(item->typeId());

                if (type == InstalledItem)
                {
                    return QColor(Qt::darkGray);
                }

                break;
            }

            default:
                break;
        }
    }

    return {};
}

bool
GtpyCollectionBrowserModel::setData(const QModelIndex& index,
                                    const QVariant& value, int role)
{
    if (!index.isValid())
    {
        return QAbstractItemModel::setData(index, value, role);
    }

    const int col = index.column();

    GtpyAbstractBrowserItem* item = static_cast<GtpyAbstractBrowserItem*>
                                    (index.internalPointer());

    if (!item->isCollapsible())
    {
        if (col == 0)
        {
            switch (role)
            {
                case Qt::CheckStateRole:
                {
                    GtpyCollectionItemType type =
                        static_cast<GtpyCollectionItemType>(item->typeId());

                    if (type == UpdateAvailableItem || type == AvailableItem)
                    {
                        Qt::CheckState state =
                            static_cast<Qt::CheckState>(value.toInt());

                        if (state == Qt::Checked)
                        {
                            item->setSelected(true);
                        }
                        else
                        {
                            item->setSelected(false);
                        }

                        emit dataChanged(index, index);
                        emit selectionChanged();
                    }

                    break;
                }

                default:
                    break;
            }
        }
    }

    return QAbstractItemModel::setData(index, value, role);
}

QVariant
GtpyCollectionBrowserModel::headerData(int section, Qt::Orientation orientation,
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
            else if (section == 3)
            {
                return tr("Installed Version");
            }

            break;

        default:
            break;
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

void
GtpyCollectionBrowserModel::setCollectionData(const
        QList<GtCollectionNetworkItem>& installedItems,
        const QList<GtCollectionNetworkItem>& availableItems,
        const QList<GtCollectionNetworkItem>& updataAvailableItems)
{
    beginResetModel();

    m_rootItem->clearRoot();

    if (!availableItems.isEmpty())
    {
        m_rootItem->createChild(AvailableItem, tr("Available"));
    }

    if (!updataAvailableItems.isEmpty())
    {
        m_rootItem->createChild(UpdateAvailableItem, tr("Update available!"));
    }

    if (!installedItems.isEmpty())
    {
        m_rootItem->createChild(InstalledItem, tr("Installed"));
    }

    foreach (GtCollectionNetworkItem item, availableItems)
    {
        appendItemTo(item, m_rootItem->child(AvailableItem));
    }

    foreach (GtCollectionNetworkItem item, updataAvailableItems)
    {
        appendItemTo(item, m_rootItem->child(UpdateAvailableItem));
    }

    foreach (GtCollectionNetworkItem item, installedItems)
    {
        appendItemTo(item, m_rootItem->child(InstalledItem));
    }

    endResetModel();

    emit selectionChanged();
}

QModelIndex
GtpyCollectionBrowserModel::index(int row, int column,
                                  const QModelIndex& parent) const
{
    if (!m_rootItem)
    {
        return {};
    }

    if (!hasIndex(row, column, parent))
    {
        return {};
    }

    GtpyAbstractBrowserItem* parentItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<GtpyAbstractBrowserItem*>(
                         parent.internalPointer());
    }

    GtpyAbstractBrowserItem* childItem = parentItem->child(row);

    if (childItem)
    {
        if (childItem->childCount() > 0 || !childItem->isCollapsible())
        {
            return createIndex(row, column, childItem);
        }
    }

    return {};
}

QModelIndex
GtpyCollectionBrowserModel::parent(const QModelIndex& index) const
{
    if (!m_rootItem)
    {
        return {};
    }

    if (!index.isValid())
    {
        return {};
    }

    GtpyAbstractBrowserItem* childItem =
        static_cast<GtpyAbstractBrowserItem*>(index.internalPointer());

    GtpyAbstractBrowserItem* parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
    {
        return {};
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

Qt::ItemFlags
GtpyCollectionBrowserModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.isValid() && index.parent().isValid() && index.column() == 0)
    {
        GtpyAbstractBrowserItem* item =
            static_cast<GtpyAbstractBrowserItem*>(index.internalPointer());

        bool collapsible = item->isCollapsible();
        GtpyCollectionItemType type = static_cast<GtpyCollectionItemType>(
                                          item->typeId());

        if (!collapsible)
        {
            switch (type)
            {
                case UpdateAvailableItem:
                case AvailableItem:
                {
                    flags = flags | Qt::ItemIsUserCheckable;
                    break;
                }

                default:
                    break;
            }
        }
    }

    return flags;
}

GtCollectionItem
GtpyCollectionBrowserModel::itemFromIndex(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return {};
    }

    if (index.model() != this)
    {
        return {};
    }

    QModelIndex parent = index.parent();

    if (!parent.isValid())
    {
        return {};
    }

    GtpyAbstractBrowserItem* item = static_cast<GtpyAbstractBrowserItem*>
                                    (index.internalPointer());

    if (!item)
    {
        return {};
    }

    if (item->isCollapsible())
    {
        return {};
    }

    return item->item();
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::selectedItems()
{
    QList<GtCollectionNetworkItem> retval;

    GtpyCollapsibleBrowserItem* item = m_rootItem->child(
                                           UpdateAvailableItem);

    if (item)
    {
        retval.append(item->selectedItems());
    }

    item = m_rootItem->child(AvailableItem);

    if (item)
    {
        retval.append(item->selectedItems());
    }

    return retval;
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::itemsToUpdate()
{
    QList<GtCollectionNetworkItem> retval;

    GtpyCollapsibleBrowserItem* item = m_rootItem->child(
                                           UpdateAvailableItem);

    if (item)
    {
        retval.append(item->uncollapsibleChilren());
    }

    return retval;
}

void
GtpyCollectionBrowserModel::selectAll()
{
    beginResetModel();

    GtpyCollapsibleBrowserItem* item = m_rootItem->child(
                                           UpdateAvailableItem);

    if (item)
    {
        item->selectAllChildren();
    }

    item = m_rootItem->child(AvailableItem);

    if (item)
    {
        item->selectAllChildren();
    }

    endResetModel();

    emit selectionChanged();
}

void
GtpyCollectionBrowserModel::unselectAll()
{
    beginResetModel();

    GtpyCollapsibleBrowserItem* item = m_rootItem->child(
                                           UpdateAvailableItem);

    if (item)
    {
        item->unselectAllChildren();
    }

    item = m_rootItem->child(AvailableItem);

    if (item)
    {
        item->unselectAllChildren();
    }

    endResetModel();
}

void
GtpyCollectionBrowserModel::appendItemTo(GtCollectionNetworkItem item,
        GtpyCollapsibleBrowserItem* to)
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

    to->appendChild(new GtpyBrowserItem(item), hierarchy);
}
