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
#include "plugin_playbackend_factory.h"
#include "plugin_playbackend_interface.h"
#include "plugin_playbackend_cabstractconfigwidget.h"
#include "plugin_playbackend_cabstractplaybackend.h"
#include "portaudio/plugin_playbackend_portaudio.h"
#include <QHash>

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
// ----------------------------------------------------------------------

QHash<QByteArray, Interface *> Factory::m_plugins;
QList<Interface *> Factory::m_pluginList;

bool Factory::init(const QString *reason)
{
    Q_UNUSED(reason);

    Interface *iface_portaudio = new Portaudio();
    foreach (const QByteArray &id, iface_portaudio->ids())
    {
        m_plugins[id] = iface_portaudio;
    }
    m_pluginList.push_back(iface_portaudio);

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

QByteArray Factory::defaultId()
{
    return m_pluginList.first()->ids().first();
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

CAbstractConfigWidget *Factory::createConfig(const QByteArray &id,
        QWidget *parent, Qt::WindowFlags f)
{
    return m_plugins[id]->createConfig(id, parent, f);
}

CAbstractPlayBackend *Factory::createBackend(const QByteArray &id,
        QObject *parent)
{
    return m_plugins[id]->createBackend(id, parent);
}

// ----------------------------------------------------------------------
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------
