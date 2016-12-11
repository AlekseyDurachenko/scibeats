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
#include "plugin_soundprovider_factory.h"
#include "plugin_soundprovider_interface.h"
#include "fdsnws/plugin_soundprovider_fdsnws_interface.h"
#include <QHash>
#include <QXmlQuery>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QtXml>


const QString shemaUrl(":///plugin/soundprovider/xsd.xml");


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
// ----------------------------------------------------------------------


QHash<QByteArray, Interface *> Factory::m_plugins;
QList<Interface *> Factory::m_pluginList;


bool Factory::init(QString *reason)
{
    if (!appendPlugin(new fdsnws::Interface(), reason)) {
        return false;
    }

    return true;
}

bool Factory::appendPlugin(Interface *iface, QString *reason)
{
    foreach (const QByteArray &id, iface->ids()) {
        if (m_plugins.contains(id)) {
            if (reason) {
                *reason = QObject::tr("duplicate id found");
            }

            return false;
        }

        m_plugins[id] = iface;
    }

    m_pluginList.push_back(iface);

    return true;
}

void Factory::destroy()
{
    foreach (Interface *iface, m_pluginList) {
        delete iface;
    }

    m_plugins.clear();
    m_pluginList.clear();
}

QList<QByteArray> Factory::ids()
{
    return m_plugins.keys();
}

QString Factory::group(const QByteArray &id)
{
    Q_ASSERT(m_plugins.contains(id));

    return m_plugins[id]->group(id);
}

QString Factory::name(const QByteArray &id)
{
    Q_ASSERT(m_plugins.contains(id));

    return m_plugins[id]->name(id);
}

QString Factory::desc(const QByteArray &id)
{
    Q_ASSERT(m_plugins.contains(id));

    return m_plugins[id]->desc(id);
}

QIcon Factory::icon(const QByteArray &id)
{
    Q_ASSERT(m_plugins.contains(id));

    return m_plugins[id]->icon(id);
}

AbstractWidget *Factory::createWidget(const QByteArray &id,
                                      QNetworkAccessManager *network,
                                      QWidget *parent,
                                      Qt::WindowFlags f)
{
    Q_ASSERT(m_plugins.contains(id));

    return m_plugins[id]->createWidget(id, network, parent, f);
}

bool Factory::validateRequest(const SoundProviderRequest &request, QString *reason)
{
    const QByteArray id = findId(request, reason);
    if (id.isEmpty()) {
        return false;
    }

    if (!m_plugins.contains(id)) {
        if (reason) {
            *reason = QObject::tr("sound provider \"%1\" is not supported").arg(QString::fromUtf8(id));
        }

        return false;
    }

    return m_plugins[id]->validateRequest(id, request, reason);
}

SoundTags Factory::tagsFromRequest(const SoundProviderRequest &request, QString *reason)
{
    const QByteArray id = findId(request, reason);
    if (id.isEmpty()) {
        return SoundTags();
    }

    if (!m_plugins.contains(id)) {
        if (reason) {
            *reason = QObject::tr("sound provider \"%1\" is not supported").arg(QString::fromUtf8(id));
        }

        return SoundTags();
    }

    return m_plugins[id]->tagsFromRequest(id, request, reason);
}

AbstractSoundProviderWorker *Factory::createWorker(const SoundProviderRequest &request,
                                                   QNetworkAccessManager *network,
                                                   QObject *parent,
                                                   QString *reason)
{
    const QByteArray id = findId(request, reason);
    if (id.isEmpty()) {
        return 0;
    }

    Q_ASSERT(m_plugins.contains(id));

    return m_plugins[id]->createWorker(id, network, parent);
}

QByteArray Factory::findId(const SoundProviderRequest &request, QString *reason)
{
    try {
        QFile file(shemaUrl);
        if (!file.open(QIODevice::ReadOnly)) {
            throw file.errorString();
        }

        QXmlSchema shema;
        if (!shema.load(&file)) {
            throw QObject::tr("can't load schema %1").arg(shemaUrl);
        }

        const QXmlSchemaValidator validator(shema);
        if (!validator.validate(request.query())) {
            throw QObject::tr("invalid request format");
        }

        QDomDocument doc;
        if (!doc.setContent(request.query())) {
            throw QObject::tr("invalid request format");
        }

        QDomElement elemSciBeats = doc.firstChildElement("SciBeats");
        QDomElement elemSoundProvider = elemSciBeats.firstChildElement("SoundProvider");

        return elemSoundProvider.text().toUtf8();
    }
    catch (const QString &errorString) {
        if (reason) {
            *reason = errorString;
        }
    }

    return QByteArray();
}


// ----------------------------------------------------------------------
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
