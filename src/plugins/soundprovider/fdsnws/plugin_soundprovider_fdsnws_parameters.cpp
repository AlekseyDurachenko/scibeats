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
#include "plugin_soundprovider_fdsnws_parameters.h"
#include <QBuffer>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QXmlQuery>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif

const QString shemaUrl(":///plugin/soundprovider/fdsnws/xsd.xml");


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
namespace fdsnws {
// ----------------------------------------------------------------------


Parameters::Parameters()
{
}

void Parameters::setUrl(const QUrl &url)
{
    m_url = url;
}

void Parameters::setNetwork(const QString &network)
{
    m_network = network;
}

void Parameters::setStation(const QString &station)
{
    m_station = station;
}

void Parameters::setLocation(const QString &location)
{
    m_location = location;
}

void Parameters::setChannel(const QString &channel)
{
    m_channel = channel;
}

void Parameters::setStartTime(const QDateTime &startTime)
{
    m_startTime = startTime;
}

void Parameters::setEndTime(const QDateTime &endTime)
{
    m_endTime = endTime;
}

void Parameters::setTags(const SoundTags &tags)
{
    m_tags = tags;
}

bool Parameters::fromXml(const QByteArray &xml, QString *reason)
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
        if (!validator.validate(xml)) {
            throw QObject::tr("invalid \"fdsnws\" request format");
        }

        QDomDocument doc;
        if (!doc.setContent(xml)) {
            throw QObject::tr("invalid request format");
        }

        QDomElement elemSciBeats = doc.firstChildElement("SciBeats");
        QDomElement elemParams = elemSciBeats.firstChildElement("Parameters");
        QDomElement elemUrl = elemParams.firstChildElement("Url");
        QDomElement elemNetwork = elemParams.firstChildElement("Network");
        QDomElement elemStation = elemParams.firstChildElement("Station");
        QDomElement elemLocation = elemParams.firstChildElement("Location");
        QDomElement elemChannel = elemParams.firstChildElement("Channel");
        QDomElement elemStartTime = elemParams.firstChildElement("StartTime");
        QDomElement elemEndTime = elemParams.firstChildElement("EndTime");
        QDomElement elemTags = elemParams.firstChildElement("Tags");

        m_url = QUrl(elemUrl.text());
        m_network = elemNetwork.text();
        m_station = elemStation.text();
        m_location = elemLocation.text();
        m_channel = elemChannel.text();
        m_startTime = QDateTime::fromString(elemStartTime.text(), Qt::ISODate);
        m_startTime.setTimeSpec(Qt::UTC);
        m_endTime = QDateTime::fromString(elemEndTime.text(), Qt::ISODate);
        m_endTime.setTimeSpec(Qt::UTC);
        m_tags = SoundTags();

        if (m_startTime >= m_endTime) {
            throw QObject::tr("invalid request format");
        }

        if (!elemTags.isNull()) {
            m_tags.setTitle(elemTags.firstChildElement("Title").text());
            m_tags.setCopyright(elemTags.firstChildElement("Copyright").text());
            m_tags.setSoftware(elemTags.firstChildElement("Software").text());
            m_tags.setArtist(elemTags.firstChildElement("Artist").text());
            m_tags.setComment(elemTags.firstChildElement("Comment").text());
            m_tags.setDate(elemTags.firstChildElement("Date").text());
            m_tags.setGenre(elemTags.firstChildElement("Genre").text());
            m_tags.setTitle(elemTags.firstChildElement("Title").text());
        }

        return true;
    }
    catch (const QString &errorString) {
        if (reason) {
            *reason = errorString;
        }
    }

    return false;
}

QByteArray Parameters::toXml() const
{
    QDomDocument doc("SciBeats-1.0");
    QDomElement sciBeats = doc.createElement("SciBeats");
    QDomElement soundProvider = doc.createElement("SoundProvider");
    QDomElement parameters = doc.createElement("Parameters");
    QDomElement paramUrl = doc.createElement("Url");
    QDomElement paramNetwork = doc.createElement("Network");
    QDomElement paramStation = doc.createElement("Station");
    QDomElement paramLocation = doc.createElement("Location");
    QDomElement paramChannel = doc.createElement("Channel");
    QDomElement paramStartTime = doc.createElement("StartTime");
    QDomElement paramEndTime = doc.createElement("EndTime");
    QDomElement tags = doc.createElement("Tags");
    QDomElement tagTitle = doc.createElement("Title");
    QDomElement tagCopyright = doc.createElement("Copyright");
    QDomElement tagSoftware = doc.createElement("Software");
    QDomElement tagArtist = doc.createElement("Artist");
    QDomElement tagComment = doc.createElement("Comment");
    QDomElement tagDate = doc.createElement("Date");
    QDomElement tagGenre = doc.createElement("Genre");

    soundProvider.appendChild(doc.createTextNode("fdsnws"));
    paramUrl.appendChild(doc.createTextNode(m_url.toString()));
    paramNetwork.appendChild(doc.createTextNode(m_network));
    paramStation.appendChild(doc.createTextNode(m_station));
    paramLocation.appendChild(doc.createTextNode(m_location));
    paramChannel.appendChild(doc.createTextNode(m_channel));
    paramStartTime.appendChild(doc.createTextNode(m_startTime.toString("yyyy-MM-ddTHH:mm:ss")));
    paramEndTime.appendChild(doc.createTextNode(m_endTime.toString("yyyy-MM-ddTHH:mm:ss")));
    tagTitle.appendChild(doc.createTextNode(m_tags.title()));
    tagCopyright.appendChild(doc.createTextNode(m_tags.copyright()));
    tagSoftware.appendChild(doc.createTextNode(m_tags.software()));
    tagArtist.appendChild(doc.createTextNode(m_tags.artist()));
    tagComment.appendChild(doc.createTextNode(m_tags.comment()));
    tagDate.appendChild(doc.createTextNode(m_tags.date()));
    tagGenre.appendChild(doc.createTextNode(m_tags.genre()));

    tags.appendChild(tagTitle);
    tags.appendChild(tagCopyright);
    tags.appendChild(tagSoftware);
    tags.appendChild(tagArtist);
    tags.appendChild(tagComment);
    tags.appendChild(tagDate);
    tags.appendChild(tagGenre);
    parameters.appendChild(paramUrl);
    parameters.appendChild(paramNetwork);
    parameters.appendChild(paramStation);
    parameters.appendChild(paramLocation);
    parameters.appendChild(paramChannel);
    parameters.appendChild(paramStartTime);
    parameters.appendChild(paramEndTime);
    parameters.appendChild(tags);
    sciBeats.appendChild(soundProvider);
    sciBeats.appendChild(parameters);
    doc.appendChild(sciBeats);

    return doc.toByteArray(4);
}

QUrl Parameters::buildQueryUrl() const
{
#if QT_VERSION >= 0x050000
    QUrl url = m_url;
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("net", m_network);
    urlQuery.addQueryItem("sta", m_station);
    urlQuery.addQueryItem("loc", m_location);
    urlQuery.addQueryItem("cha", m_channel);
    urlQuery.addQueryItem("starttime", m_startTime.toString("yyyy-MM-ddTHH:mm:ss"));
    urlQuery.addQueryItem("endtime", m_endTime.toString("yyyy-MM-ddTHH:mm:ss"));
    url.setQuery(urlQuery);
    return url;
#else
    QUrl url = m_url;
    url.addQueryItem("net", m_network);
    url.addQueryItem("sta", m_station);
    url.addQueryItem("loc", m_location);
    url.addQueryItem("cha", m_channel);
    url.addQueryItem("starttime", m_startTime.toString("yyyy-MM-ddTHH:mm:ss"));
    url.addQueryItem("endtime", m_endTime.toString("yyyy-MM-ddTHH:mm:ss"));
    return url;
#endif
}

bool Parameters::validate(const QByteArray &xml, QString *reason)
{
    Parameters params;
    return params.fromXml(xml, reason);
}


// ----------------------------------------------------------------------
} // namespace fdsnws
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------
