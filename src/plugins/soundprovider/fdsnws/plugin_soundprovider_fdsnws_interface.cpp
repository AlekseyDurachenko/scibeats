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
#include "plugin_soundprovider_fdsnws_interface.h"
#include "plugin_soundprovider_fdsnws_parameters.h"
#include "plugin_soundprovider_fdsnws_widget.h"
#include "plugin_soundprovider_fdsnws_worker.h"

#include <QXmlSchema>
#include <QXmlSchemaValidator>


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
namespace fdsnws {
// ----------------------------------------------------------------------


Interface::Interface()
{
}

Interface::~Interface()
{
}

QList<QByteArray> Interface::ids() const
{
    return QList<QByteArray>() << QByteArray("fdsnws");
}

QString Interface::group(const QByteArray &id) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return QObject::tr("Seismic");
    }

    return QString();
}

QString Interface::name(const QByteArray &id) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return QObject::tr("FDSNWS");
    }

    return QString();
}

QString Interface::desc(const QByteArray &id) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return QObject::tr("Download seismic waves through FDSN Web Serivce");
    }

    return QString();
}

QIcon Interface::icon(const QByteArray &id) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return QIcon();
    }

    return QIcon();
}

plugin::soundprovider::AbstractWidget *Interface::createWidget(const QByteArray &id,
                                                               QNetworkAccessManager *network,
                                                               QWidget *parent,
                                                               Qt::WindowFlags f) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return new Widget(network, parent, f);
    }

    return 0;
}

bool Interface::validateRequest(const QByteArray &id,
                                const SoundProviderRequest &request,
                                QString *reason) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return Parameters::validate(request.query(), reason);
    }

    return false;
}

SoundTags Interface::tagsFromRequest(const QByteArray &id,
                                     const SoundProviderRequest &request,
                                     QString *reason) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        Parameters params;
        if (!params.fromXml(request.query(), reason)) {
            return SoundTags();
        }

        return params.tags();
    }

    return SoundTags();
}

AbstractSoundProviderWorker *Interface::createWorker(const QByteArray &id,
                                                     QNetworkAccessManager *network,
                                                     QObject *parent) const
{
    Q_ASSERT(ids().contains(id));

    if (id == QByteArray("fdsnws")) {
        return new Worker(network, parent);
    }

    return 0;
}


// ----------------------------------------------------------------------
} // namespace fdsnws
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------
