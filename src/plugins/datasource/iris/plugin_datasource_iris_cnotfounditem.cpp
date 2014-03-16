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
#include "plugin_datasource_iris_cnotfounditem.h"
#include <QIcon>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CNotFoundItem::CNotFoundItem(int row, QObject *parent) :
    CAbstractItem(NotFound, row, parent)
{
}

QVariant CNotFoundItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole && column == 0)
    {
        CAbstractItem::ItemType itemType = qobject_cast<CAbstractItem *>(parent())->itemType();
        switch (itemType)
        {
        case CAbstractItem::Query:
            return tr("The events for this query not found");
        case CAbstractItem::Event:
            return tr("The traces for this event not found");
        default:
            return tr("Item not found");
        }
    }
    else if (role == Qt::DecorationRole && column == 0)
    {
        return QIcon(":/plugin/datasource/iris/icons/status-notfound.png");
    }

    return QVariant();
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
