// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "plugin_datasource_iris_citemmodel.h"
#include "plugin_datasource_iris_cprogressitem.h"
#include <QDebug>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CItemModel::CItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = 0;
}

QVariant CItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootItem)
    {
        return QVariant();
    }

    return static_cast<CAbstractItem *>(index.internalPointer())->data(index.column(), role);
}

Qt::ItemFlags CItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return 0;
    }

    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant CItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!m_rootItem)
    {
        return QVariant();
    }

    return m_rootItem->headerData(section, orientation, role);
}

QModelIndex CItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || !m_rootItem)
    {
        return QModelIndex();
    }

    CAbstractItem *parentItem = m_rootItem;
    if (parent.isValid())
    {
        parentItem = static_cast<CAbstractItem *>(parent.internalPointer());
    }

    return createIndex(row, column, parentItem->child(row));
}

QModelIndex CItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid() || !m_rootItem)
    {
        return QModelIndex();
    }

    CAbstractItem *childItem = static_cast<CAbstractItem*>(index.internalPointer());
    CAbstractItem *parentItem = static_cast<CAbstractItem*>(childItem->parent());

    if (parentItem == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int CItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0 || !m_rootItem)
    {
        return 0;
    }

    CAbstractItem *parentItem = m_rootItem;
    if (parent.isValid())
    {
        parentItem = static_cast<CAbstractItem *>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int CItemModel::columnCount(const QModelIndex &parent) const
{
    if (!m_rootItem)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        return m_rootItem->columnCount();
    }

    return static_cast<CAbstractItem *>(parent.internalPointer())->columnCount();
}

void CItemModel::setRootItem(CAbstractItem *rootItem)
{
    beginResetModel();
    m_rootItem = rootItem;
    connect(m_rootItem, SIGNAL(rowInserted(CAbstractItem *,int,int)),
            this, SLOT(slot_rowInserted(CAbstractItem *,int,int)));
    connect(m_rootItem, SIGNAL(rowRemoved(CAbstractItem *,int,int)),
            this, SLOT(slot_rowRemoved(CAbstractItem *,int,int)));
    connect(m_rootItem, SIGNAL(dataChanged(CAbstractItem *,int,int)),
            this, SLOT(slot_dataChanged(CAbstractItem *,int,int)));
    endResetModel();
}

void CItemModel::slot_rowInserted(CAbstractItem *parent, int first, int last)
{
    if (parent == m_rootItem)
    {
        beginInsertRows(QModelIndex(), first, last);
    }
    else
    {
        beginInsertRows(createIndex(parent->row(), 0, parent), first, last);
    }
    endInsertRows();
}

void CItemModel::slot_rowRemoved(CAbstractItem *parent, int first, int last)
{
    if (parent == m_rootItem)
    {
        beginRemoveRows(QModelIndex(), first, last);
    }
    else
    {
        beginRemoveRows(createIndex(parent->row(), 0, parent), first, last);
    }
    endRemoveRows();
}

void CItemModel::slot_dataChanged(CAbstractItem *parent, int first, int last)
{
    emit dataChanged(createIndex(first, 0, parent->parent()),
            createIndex(last, parent->columnCount(), parent->parent()));
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
