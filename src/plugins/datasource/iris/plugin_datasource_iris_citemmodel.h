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
#ifndef PLUGIN_DATASOURCE_IRIS_CITEMMODEL_H
#define PLUGIN_DATASOURCE_IRIS_CITEMMODEL_H

#include "plugin_datasource_iris_cabstractitem.h"
#include <QAbstractItemModel>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

class CItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CItemModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void setRootItem(CAbstractItem *rootItem);

private slots:
    void slot_rowInserted(CAbstractItem *parent, int first, int last);
    void slot_rowRemoved(CAbstractItem *parent, int first, int last);
    void slot_dataChanged(CAbstractItem *parent, int first, int last);

private:
    CAbstractItem *m_rootItem;

};

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_CITEMMODEL_H
