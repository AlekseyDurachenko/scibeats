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
#include "plugin_datasource_iris_cabstractitem.h"
#include <QVariant>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CAbstractItem::CAbstractItem(ItemType itemType, int row, QObject *parent) :
    QObject(parent), m_itemType(itemType), m_row(row)
{
}

int CAbstractItem::columnCount() const
{
    return 6;
}

int CAbstractItem::childCount() const
{
    return 0;
}

QVariant CAbstractItem::headerData(int section,
        Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return tr("Id");
        }
        else if (section == 1)
        {
            return tr("Magnitude/Distance");
        }
        else if (section == 2)
        {
            return tr("Time");
        }
        else if (section == 3)
        {
            return tr("Latitude");
        }
        else if (section == 4)
        {
            return tr("Longitude");
        }
        else if (section == 5)
        {
            return tr("Site name");
        }
    }

    return QVariant();
}

QVariant CAbstractItem::data(int, int) const
{
    return QVariant();
}

CAbstractItem *CAbstractItem::child(int)
{
    return 0;
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
