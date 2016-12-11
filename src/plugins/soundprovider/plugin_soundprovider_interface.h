// Copyright 2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef PLUGIN_SOUNDPROVIDER_INTERFACE_H
#define PLUGIN_SOUNDPROVIDER_INTERFACE_H


#include "soundtags.h"
#include "plugin_soundprovider_abstractwidget.h"
#include "abstractsoundproviderworker.h"
#include <QIcon>
#include <QDomNode>


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
// ----------------------------------------------------------------------


class Interface
{
public:
    Interface() {}
    virtual ~Interface() {}
    //
    virtual QList<QByteArray> ids() const = 0; // should be unique id
    virtual QString group(const QByteArray &id) const = 0;
    virtual QString name(const QByteArray &id) const = 0;
    virtual QString desc(const QByteArray &id) const = 0;
    virtual QIcon icon(const QByteArray &id) const = 0;
    virtual AbstractWidget *createWidget(const QByteArray &id,
                                         QNetworkAccessManager *network,
                                         QWidget *parent,
                                         Qt::WindowFlags f = 0) const = 0;
    virtual bool validateRequest(const QByteArray &id,
                                 const SoundProviderRequest &request,
                                 QString *reason = 0) const = 0;
    virtual SoundTags tagsFromRequest(const QByteArray &id,
                                      const SoundProviderRequest &request,
                                      QString *reason = 0) const = 0;
    virtual AbstractSoundProviderWorker *createWorker(const QByteArray &id,
                                                      QNetworkAccessManager *network,
                                                      QObject *parent = 0) const = 0;
};


// ----------------------------------------------------------------------
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------


#endif // PLUGIN_SOUNDPROVIDER_INTERFACE_H
