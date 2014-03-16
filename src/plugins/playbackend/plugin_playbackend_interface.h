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
#ifndef PLUGIN_PLAYBACKEND_INTERFACE_H
#define PLUGIN_PLAYBACKEND_INTERFACE_H

#include <QIcon>
#include "plugin_playbackend_cabstractconfigwidget.h"
#include "plugin_playbackend_cabstractplaybackend.h"

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
// ----------------------------------------------------------------------

class Interface
{
public:
    Interface() {}
    //
    virtual QList<QByteArray> ids() const = 0; // should be unique id
    virtual QString name(const QByteArray& id) const = 0;
    virtual QString desc(const QByteArray& id) const = 0;
    virtual QIcon icon(const QByteArray& id) const = 0;
    virtual CAbstractConfigWidget *createConfig(const QByteArray& id,
            QWidget *parent, Qt::WindowFlags f = 0) const = 0;
    virtual CAbstractPlayBackend *createBackend(const QByteArray& id,
            QObject *parent) const = 0;

};

// ----------------------------------------------------------------------
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_PLAYBACKEND_INTERFACE_H
