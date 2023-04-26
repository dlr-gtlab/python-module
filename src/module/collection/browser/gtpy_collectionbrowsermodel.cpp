/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionbrowsermodel.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 19.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_constants.h"
#include "gtpy_icons_compat.h"
#include "gtpy_browseritem.h"
#include "gtpy_collapsiblebrowseritem.h"
#include "gtpy_rootbrowseritem.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_colors.h"
#endif

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

    if (parent.column() > 0)
    {
        return 0;
    }

    return !parent.isValid() ? m_rootItem->childCount() :
                               indexToBrowserItem(parent)->childCount();
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

    auto item = indexToBrowserItem(index);
    const int col = index.column();

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

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
        case Qt::BackgroundRole:
            if (item == m_rootItem->child(UpdateAvailableItem))
            {
                return gt::gui::color::collectionAvailableItemBackground();
            }

            break;

        case Qt::ForegroundRole:

            if (item->typeId() == InstalledItem &&
                    item != m_rootItem->child(InstalledItem))
            {
                return gt::gui::color::disabled();
            }

            break;

        default:
            break;
#else
        case Qt::BackgroundRole:
            if (item == m_rootItem->child(UpdateAvailableItem))
            {
                return QColor(180, 229, 190);
            }
            else if (item == m_rootItem->child(AvailableItem))
            {
                return QColor(255, 255, 255);
            }
            else if (item == m_rootItem->child(InstalledItem))
            {
                return QColor(240, 240, 240);;
            }
            else if (item->parentItem() ==
                     m_rootItem->child(UpdateAvailableItem))
            {
                return QColor(245, 245, 245);
            }
            else if (item->parentItem() == m_rootItem->child(AvailableItem))
            {
                return QColor(245, 245, 245);
            }
            else if (item->parentItem() == m_rootItem->child(InstalledItem))
            {
                return QColor(245, 245, 245);
            }
            else
            {
                return QColor(250, 250, 250);;
            }
#endif
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
                    if (item->typeId() == UpdateAvailableItem ||
                            item->typeId() == InstalledItem)
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
                    if (item->typeId() == UpdateAvailableItem ||
                            item->typeId() == AvailableItem)
                    {
                        if (item->isSelected())
                        {
                            return Qt::Checked;
                        }

                        return Qt::Unchecked;
                    }
                }

                break;

            case Qt::ForegroundRole:

                if (item->typeId() == InstalledItem)
                {
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
                    return gt::gui::color::disabled();
#else
                    return QColor(Qt::darkGray);
#endif
                }

                break;

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

    auto item = indexToBrowserItem(index);
    const int col = index.column();

    if (!item->isCollapsible())
    {
        if (col == 0)
        {
            switch (role)
            {
                case Qt::CheckStateRole:
                {
                    if (item->typeId() == UpdateAvailableItem ||
                            item->typeId() == AvailableItem)
                    {
                        if (static_cast<Qt::CheckState>(value.toInt()) ==
                                Qt::Checked)
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

    auto parentItem = !parent.isValid() ? m_rootItem :
                                          indexToBrowserItem(parent);

    auto childItem = parentItem->child(row);

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

    auto parent = indexToBrowserItem(index)->parentItem();

    return parent != m_rootItem ? createIndex(parent->row(), 0, parent) :
                                  QModelIndex{};
}

Qt::ItemFlags
GtpyCollectionBrowserModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.isValid() && index.parent().isValid() && index.column() == 0)
    {
        auto item = indexToBrowserItem(index);

        if (!item->isCollapsible())
        {
            switch (item->typeId())
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

    auto item = indexToBrowserItem(index);

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

    if (auto item = m_rootItem->child(UpdateAvailableItem))
    {
        retval.append(item->selectedItems());
    }

    if (auto item = m_rootItem->child(AvailableItem))
    {
        retval.append(item->selectedItems());
    }

    return retval;
}

QList<GtCollectionNetworkItem>
GtpyCollectionBrowserModel::itemsToUpdate()
{
    QList<GtCollectionNetworkItem> retval;

    if (auto item = m_rootItem->child(UpdateAvailableItem))
    {
        retval.append(item->uncollapsibleChilren());
    }

    return retval;
}

void
GtpyCollectionBrowserModel::selectAll()
{
    beginResetModel();

    if (auto item = m_rootItem->child(UpdateAvailableItem))
    {
        item->selectAllChildren();
    }

    if (auto item = m_rootItem->child(AvailableItem))
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

    if (auto item = m_rootItem->child(UpdateAvailableItem))
    {
        item->unselectAllChildren();
    }

    if (auto item = m_rootItem->child(AvailableItem))
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

    auto cat = item.property(gtpy::constants::COLLECTION_CAT).toString();

    if (!cat.isEmpty())
    {
        hierarchy << cat;

        cat = item.property(gtpy::constants::COLLECTION_SUBCAT).toString();

        if (!cat.isEmpty())
        {
            hierarchy << cat;
        }
    }

    to->appendChild(new GtpyBrowserItem(item), hierarchy);
}

GtpyAbstractBrowserItem*
GtpyCollectionBrowserModel::indexToBrowserItem(const QModelIndex& index) const
{
    return static_cast<GtpyAbstractBrowserItem*>(index.internalPointer());
}
