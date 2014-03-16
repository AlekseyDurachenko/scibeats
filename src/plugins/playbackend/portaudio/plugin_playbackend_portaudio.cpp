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
#include "plugin_playbackend_portaudio.h"
#include "plugin_playbackend_portaudio_cconfigwidget.h"
#include "plugin_playbackend_portaudio_cplaybackend.h"

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
// ----------------------------------------------------------------------

Portaudio::Portaudio()
{
}

QList<QByteArray> Portaudio::ids() const
{
    return QList<QByteArray>() << "plugin_playbackend_portaudio";
}

QString Portaudio::name(const QByteArray &id) const
{
    if (id == "plugin_playbackend_portaudio")
    {
        return QObject::tr("PortAudio backend");
    }

    return QString();
}

QString Portaudio::desc(const QByteArray &id) const
{
    if (id == "plugin_playbackend_portaudio")
    {
        return QObject::tr("PortAudio backend");
    }

    return QString();
}

QIcon Portaudio::icon(const QByteArray &id) const
{
    if (id == "plugin_playbackend_portaudio")
    {
        return QIcon();
    }

    return QIcon();
}

CAbstractConfigWidget *plugin::playbackend::Portaudio::createConfig(
    const QByteArray &id, QWidget *parent, Qt::WindowFlags f) const
{
    if (id == "plugin_playbackend_portaudio")
    {
        return new portaudio::CConfigWidget(parent, f);
    }

    return 0;
}

CAbstractPlayBackend *plugin::playbackend::Portaudio::createBackend(
    const QByteArray &id, QObject *parent) const
{
    if (id == "plugin_playbackend_portaudio")
    {
        return new portaudio::CPlayBackend(parent);
    }

    return 0;
}

// ----------------------------------------------------------------------
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------
