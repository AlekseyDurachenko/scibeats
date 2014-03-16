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
#include "plugin_datasource_iris.h"
#include "plugin_datasource_iris_ccontrolwidget.h"

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
// ----------------------------------------------------------------------
using namespace iris;

Iris::Iris()
{
}

QList<QByteArray> Iris::ids() const
{
    return QList<QByteArray>() << "plugin_datasource_irissimple";
}

QString Iris::name(const QByteArray& id) const
{
    if (id == "plugin_datasource_irissimple")
    {
        return QObject::tr("IRIS DMC Web Services");
    }

    return QString();
}

QString Iris::desc(const QByteArray& id) const
{
    if (id == "plugin_datasource_irissimple")
    {
        return QObject::tr("See the http://service.iris.edu/ for details");
    }

    return QString();
}

QIcon Iris::icon(const QByteArray& id) const
{
    if (id == "plugin_datasource_irissimple")
    {
        return QIcon();
    }

    return QIcon();
}

CAbstractWidget *Iris::create(const QByteArray& id,
        QNetworkAccessManager *network, QWidget *parent, Qt::WindowFlags f) const
{
    if (id == "plugin_datasource_irissimple")
    {
        return new CControlWidget(network, parent, f);
    }

    return 0;
}

// ----------------------------------------------------------------------
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
