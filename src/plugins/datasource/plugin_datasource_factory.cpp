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
#include "plugin_datasource_factory.h"
#include "iris/plugin_datasource_iris.h"
#include <QHash>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
// ----------------------------------------------------------------------

QHash<QByteArray, Interface *> Factory::m_plugins;
QList<Interface *> Factory::m_pluginList;

bool Factory::init(const QString *reason)
{
    Q_UNUSED(reason);

    Interface *iface_iris = new Iris();
    foreach (const QByteArray &id, iface_iris->ids())
    {
        m_plugins[id] = iface_iris;
    }
    m_pluginList.push_back(iface_iris);

    return true;
}

bool Factory::destroy(const QString *reason)
{
    Q_UNUSED(reason);

    qDeleteAll(m_pluginList);
    m_plugins.clear();
    m_pluginList.clear();

    return true;
}

QList<QByteArray> Factory::ids()
{
    return m_plugins.keys();
}

QString Factory::name(const QByteArray &id)
{
    return m_plugins[id]->name(id);
}

QString Factory::desc(const QByteArray &id)
{
    return m_plugins[id]->desc(id);
}

QIcon Factory::icon(const QByteArray &id)
{
    return m_plugins[id]->icon(id);
}

CAbstractWidget *Factory::create(const QByteArray &id,
        QNetworkAccessManager *network, QWidget *parent, Qt::WindowFlags f)
{
    return m_plugins[id]->create(id, network, parent, f);
}

// ----------------------------------------------------------------------
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
