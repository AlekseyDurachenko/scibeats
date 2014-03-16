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
#ifndef PLUGIN_DATASOURCE_IRIS_CABSTRACTITEM_H
#define PLUGIN_DATASOURCE_IRIS_CABSTRACTITEM_H

#include <QObject>
#include <QModelIndex>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

class CAbstractItem : public QObject
{
    Q_OBJECT
public:
    enum ItemType {Query, Event, Trace, Progress, NotFound};
    explicit CAbstractItem(ItemType itemType, int row, QObject *parent = 0);
    inline ItemType itemType() const;

    virtual int columnCount() const;
    virtual int childCount() const;
    virtual QVariant headerData(int section,
            Qt::Orientation orientation, int role) const;
    virtual QVariant data(int column, int role) const;
    virtual CAbstractItem *child(int row);
    inline int row() const;

signals:
    void rowInserted(CAbstractItem *parent, int first, int last);
    void rowRemoved(CAbstractItem *parent, int first, int last);
    void dataChanged(CAbstractItem *parent, int first, int last);
    void errorHappend(CAbstractItem *item, const QString &errorString);

private:
    ItemType m_itemType;
    int m_row;

};

CAbstractItem::ItemType CAbstractItem::itemType() const
{
    return m_itemType;
}

int CAbstractItem::row() const
{
    return m_row;
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_CABSTRACTITEM_H
