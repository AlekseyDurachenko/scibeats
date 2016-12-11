// Copyright 2016, Durachenko Aleksey V. <durachenko@gs.sbras.ru>
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
#include "logmessageitemmodel.h"


LogMessageItemModel::LogMessageItemModel(LogMessageManager *mgr, QObject *parent)
    : QAbstractItemModel(parent)
    , m_mgr(mgr)
{
    connect(m_mgr, SIGNAL(aboutToBeInserted(int, int)),
            this, SLOT(mgr_aboutToBeInserted(int, int)));
    connect(m_mgr, SIGNAL(inserted(int, int)),
            this, SLOT(mgr_inserted(int, int)));
    connect(m_mgr, SIGNAL(aboutToBeRemoved(int, int)),
            this, SLOT(mgr_aboutToBeRemoved(int, int)));
    connect(m_mgr, SIGNAL(removed(int, int)),
            this, SLOT(mgr_removed(int, int)));
    connect(m_mgr, SIGNAL(destroyed()),
            this, SLOT(mgr_destroyed()));
}

QVariant LogMessageItemModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Time");
        case 1:
            return tr("Message");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QVariant LogMessageItemModel::data(const QModelIndex &index, int role) const
{
    if (!m_mgr) {
        return QVariant();
    }

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const LogMessage &data = m_mgr->at(index.row());
        switch (index.column()) {
        case 0:
            return data.dateTime().toString("yyyy-MM-dd HH:mm:ss");
        case 1:
            return data.message();
        default:
            return QVariant();
        }
    }

    if (role == Qt::UserRole) {
        return index.row();
    }

    return QVariant();
}

Qt::ItemFlags LogMessageItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex LogMessageItemModel::index(int row,
                                      int column,
                                      const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column);
}

QModelIndex LogMessageItemModel::parent(const QModelIndex &/*index*/) const
{
    return QModelIndex();
}

int LogMessageItemModel::rowCount(const QModelIndex &parent) const
{
    if (!m_mgr) {
        return 0;
    }

    if (parent.column() > 0 || parent.isValid()) {
        return 0;
    }

    return m_mgr->count();
}

int LogMessageItemModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

void LogMessageItemModel::mgr_aboutToBeInserted(int first, int last)
{
    beginInsertRows(QModelIndex(), first, last);
}

void LogMessageItemModel::mgr_inserted(int /*first*/, int /*last*/)
{
    endInsertRows();
}

void LogMessageItemModel::mgr_aboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void LogMessageItemModel::mgr_removed(int /*first*/, int /*last*/)
{
    endRemoveRows();
}

void LogMessageItemModel::mgr_destroyed()
{
    beginResetModel();
    m_mgr = 0;
    endResetModel();
}
