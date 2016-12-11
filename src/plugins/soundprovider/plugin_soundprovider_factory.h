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
#ifndef PLUGIN_SOUNDPROVIDER_FACTORY_H
#define PLUGIN_SOUNDPROVIDER_FACTORY_H


#include "abstractsoundproviderworker.h"
#include "plugin_soundprovider_abstractwidget.h"
#include <QIcon>


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
// ----------------------------------------------------------------------


class Interface;
class Factory
{
public:
    static bool init(QString *reason = 0);
    static void destroy();
    static QList<QByteArray> ids();
    static QString group(const QByteArray &id);
    static QString name(const QByteArray &id);
    static QString desc(const QByteArray &id);
    static QIcon icon(const QByteArray &id);

    static bool validateRequest(const SoundProviderRequest &request, QString *reason = 0);

    static SoundTags tagsFromRequest(const SoundProviderRequest &request, QString *reason = 0);

    static AbstractWidget *createWidget(const QByteArray &id,
                                        QNetworkAccessManager *network,
                                        QWidget *parent = 0,
                                        Qt::WindowFlags f = 0);

    static AbstractSoundProviderWorker *createWorker(const SoundProviderRequest &request,
                                                     QNetworkAccessManager *network,
                                                     QObject *parent = 0,
                                                     QString *reason = 0);

    static QByteArray findId(const SoundProviderRequest &request, QString *reason = 0);

private:
    static bool appendPlugin(Interface *iface, QString *reason);

private:
    static QHash<QByteArray, Interface *> m_plugins;
    static QList<Interface *> m_pluginList;
};


// ----------------------------------------------------------------------
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------


#endif // PLUGIN_SOUNDPROVIDER_FACTORY_H
