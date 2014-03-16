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
#include "plugin_datasource_iris_ceventitem.h"
#include "plugin_datasource_iris_cqueryitem.h"
#include "plugin_datasource_iris_ctraceitem.h"
#include "plugin_datasource_iris_cprogressitem.h"
#include "plugin_datasource_iris_cnotfounditem.h"
#include "plugin_datasource_iris_utils.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QIcon>
#include <QDebug>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CEventItem::CEventItem(const QDomElement& eventElem, int row, QObject *parent) :
    CAbstractItem(Event, row, parent)
{
    m_eventId   = 0;
    m_magnitude = 0.0;
    m_latitude  = 0.0;
    m_longitude = 0.0;
    setXml(eventElem);

    m_loaded = false;
    m_reply = 0;
}

int CEventItem::childCount() const
{
    if (!m_loaded)
    {
        return 1;
    }

    return m_traceList.count();
}

QVariant CEventItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
        {
            return QString::number(m_eventId);
        }        
        else if (column == 1)
        {
            return QString::number(m_magnitude, 'f', 1);
        }
        else if (column == 2)
        {
            return m_dateTime.toString("yyyy-MM-ddTHH:mm:ss");
        }
        else if (column == 3)
        {
            return QString::number(m_latitude, 'f', 6);
        }
        else if (column == 4)
        {
            return QString::number(m_longitude, 'f', 6);
        }
        else if (column == 5)
        {
            return m_siteName;
        }
    }
    else if (role == Qt::DecorationRole && column == 0)
    {
        return QIcon(":/plugin/datasource/iris/icons/item-event.png");
    }

    return QVariant();
}

CAbstractItem *CEventItem::child(int index)
{
    if (m_loaded == false)
    {
        // cancel previouse query
        if (m_reply) m_reply->abort();
        clearTraceList();

        // create new query
        QUrl queryUrl("http://service.iris.edu/fdsnws/station/1/query");
        queryUrl.addQueryItem("format", "xml");
        queryUrl.addQueryItem("nodata", "404");
        queryUrl.addQueryItem("matchtimeseries", "TRUE");
        queryUrl.addQueryItem("level", "channel");
        queryUrl.addQueryItem("channel", "?H?");
        queryUrl.addQueryItem("latitude", QString::number(m_latitude, 'f', 6));
        queryUrl.addQueryItem("longitude", QString::number(m_longitude, 'f', 6));
        queryUrl.addQueryItem("starttime", m_dateTime.toString("yyyy-MM-ddTHH:mm:ss"));
        queryUrl.addQueryItem("endtime", m_dateTime.addSecs(10).toString("yyyy-MM-ddTHH:mm:ss"));
        queryUrl.addQueryItem("maxradius", QString::number(queryItem()->maxRadius(), 'f', 6));

        // threre should be log output
        qDebug() << queryUrl.toString();

        // show progress bar
        CProgressItem *progressItem = new CProgressItem(0, this);
        m_reply = queryItem()->network()->get(QNetworkRequest(queryUrl));
        connect(m_reply, SIGNAL(finished()), this, SLOT(slot_reply_Finished()));
        connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), progressItem, SLOT(setProgress(qint64,qint64)));
        addTraceItem(progressItem);
        emit rowInserted(this, 0, 0);

        // mark as loaded, will show the progress bar
        m_loaded = true;
    }

    return m_traceList[index];
}

void CEventItem::slot_reply_Finished()
{
    clearTraceList();

    // parse the xml reply
    if (m_reply->error() == QNetworkReply::NoError)
    {
        QString reason;
        if (!parceXml(m_reply->readAll(), &reason))
        {
            emit errorHappend(this, reason);
        }
    }
    else if (m_reply->error() != QNetworkReply::ContentNotFoundError
            && m_reply->error() != QNetworkReply::OperationCanceledError)
    {
        emit errorHappend(this, m_reply->errorString());
    }

    // analyze the result of the xml parsing
    switch (m_reply->error())
    {
    case QNetworkReply::NoError:
    case QNetworkReply::ContentNotFoundError:
    {
        if (m_traceList.count() == 0)
        {
            addTraceItem(new CNotFoundItem(0, this));
        }
        emit rowInserted(this, 0, m_traceList.count());
        break;
    }
    default:
        // the problems...
        m_loaded = false;
        emit rowInserted(this, 0, 0);
        break;
    }

    m_reply->deleteLater();
    m_reply = 0;
}

CQueryItem *CEventItem::queryItem() const
{
    return qobject_cast<CQueryItem *>(parent());
}

// The xml structure example:
//
//<FDSNStationXML xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://www.fdsn.org/xml/station/1" schemaVersion="1.0" xsi:schemaLocation="http://www.fdsn.org/xml/station/1 http://www.fdsn.org/xml/station/fdsn-station-1.0.xsd">
//  <Source>IRIS-DMC</Source>
//  <Sender>IRIS-DMC</Sender>
//  <Module>IRIS WEB SERVICE: fdsnws-station | version: 1.0.9</Module>
//  <ModuleURI>
//    http://service.iris.edu/fdsnws/station/1/query?starttime=2014-01-01T16:03:30&endtime=2014-01-01T16:03:30&level=channel&matchtimeseries=true&format=xml&channel=?H?&longitude=167.2016&latitude=-13.8732&maxradius=2
//  </ModuleURI>
//  <Created>2014-01-12T05:00:09</Created>
//  <Network code="G" startDate="1982-01-01T00:00:00" endDate="2500-12-12T23:59:59" restrictedStatus="open">
//  <Description>GEOSCOPE</Description>
//  <TotalNumberStations>55</TotalNumberStations>
//  <SelectedNumberStations>1</SelectedNumberStations>
//  <Station code="SANVU" startDate="2011-11-01T00:00:00" endDate="2599-12-31T23:59:59" restrictedStatus="open">
//    <Comment>
//      <Value>Station is down.</Value>
//      <BeginEffectiveTime>2012-01-22T11:00:00</BeginEffectiveTime>
//      <EndEffectiveTime>2012-01-27T01:00:00</EndEffectiveTime>
//    </Comment>
//    <Latitude>-15.447148</Latitude>
//    <Longitude>167.203231</Longitude>
//    <Elevation>56.0</Elevation>
//    <Site>
//      <Name>Espiritu Santo, Vanuatu</Name>
//    </Site>
//    <CreationDate>2011-11-01T00:00:00</CreationDate>
//    <TotalNumberChannels>17</TotalNumberChannels>
//    <SelectedNumberChannels>12</SelectedNumberChannels>
//    <Channel locationCode="00" startDate="2011-11-01T00:00:00" restrictedStatus="open" endDate="2599-12-31T23:59:59" code="BHE">
//    ...
//    </Channel>
//  </Station>
//</FDSNStationXML>
bool CEventItem::parceXml(QByteArray xml, QString *reason)
{
    try
    {
        QDomDocument doc("FDSNStationXML");
        if (!doc.setContent(xml))
        {
            throw tr("Unsupported XML format", "CEventItem::parceXml");
        }

        // we cannot parce the unknow xml format
        QDomElement rootElem = doc.documentElement();
        if (rootElem.nodeName() != "FDSNStationXML")
        {
            throw tr("Unsupported XML format", "CEventItem::parceXml");
        }

        int index = -1;
        QDomNode rootNode = rootElem.firstChild();
        while (!rootNode.isNull())
        {
            if (rootNode.nodeName() == "Network")
            {
                QString networkCode = rootNode.toElement().attribute("code");
                QDomNode networkNode = rootNode.firstChild();
                while (!networkNode.isNull())
                {
                    if (networkNode.nodeName() == "Station")
                    {
                        QString siteName;
                        QString stationCode = networkNode.toElement().attribute("code");
                        QDomNode stationNode = networkNode.firstChild();
                        while (!stationNode.isNull())
                        {
                            if (stationNode.nodeName() == "Site")
                            {
                                siteName = extractValueFromXmlPath(stationNode, "Name").trimmed();
                            }
                            else if (stationNode.nodeName() == "Channel")
                            {
                                addTraceItem(new CTraceItem(stationNode.toElement(), networkCode,
                                        stationCode, siteName, ++index, this));
                            }
                            stationNode = stationNode.nextSibling();
                        }
                    }
                    networkNode = networkNode.nextSibling();
                }
            }
            rootNode = rootNode.nextSibling();
        }

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

// The xml structure example:
//
//<event xmlns:iris="http://service.iris.edu/fdsnws/event/1/" publicID="smi:service.iris.edu/fdsnws/event/1/query?eventid=4218658">
//  <type>earthquake</type>
//  <description iris:FEcode="663">
//    <type>Flinn-Engdahl region</type>
//    <text>SEA OF OKHOTSK</text>
//  </description>
//  <preferredMagnitudeID>
//    smi:service.iris.edu/fdsnws/event/1/query?magnitudeid=16614615
//  </preferredMagnitudeID>
//  <preferredOriginID>
//    smi:service.iris.edu/fdsnws/event/1/query?originid=9910843
//  </preferredOriginID>
//  <origin publicID="smi:service.iris.edu/fdsnws/event/1/query?originid=9910843" iris:contributor="NEIC PDE-M" iris:catalog="NEIC PDE">
//    <time>
//      <value>2013-05-24T05:44:48.9800</value>
//    </time>
//    <creationInfo>
//      <author>NEIC</author>
//    </creationInfo>
//    <latitude>
//      <value>54.892</value>
//    </latitude>
//    <longitude>
//      <value>153.221</value>
//    </longitude>
//    <depth>
//      <value>598100.0</value>
//    </depth>
//  </origin>
//  <magnitude publicID="smi:service.iris.edu/fdsnws/event/1/query?magnitudeid=16614615">
//    <originID>
//      smi:service.iris.edu/fdsnws/event/1/query?originid=9910843
//    </originID>
//    <type>MW</type>
//    <mag>
//      <value>8.4</value>
//    </mag>
//    <creationInfo>
//      <author>UCMT</author>
//    </creationInfo>
//  </magnitude>
//</event>
void CEventItem::setXml(const QDomElement &eventElem)
{
    QString preferredMagnitudeID;
    QString preferredOriginID;

    m_eventId = eventElem.attribute("publicID").split("eventid=").at(1).toInt();

    QDomNode eventNode = eventElem.firstChild();
    while (!eventNode.isNull())
    {
        if (eventNode.nodeName() == "description")
        {
            m_siteName = extractValueFromXmlPath(eventNode, "text").trimmed();
        }
        else if (eventNode.nodeName() == "preferredMagnitudeID")
        {
            preferredMagnitudeID = eventNode.toElement().text();
        }
        else if (eventNode.nodeName() == "preferredOriginID")
        {
            preferredOriginID = eventNode.toElement().text();
        }
        else if (eventNode.nodeName() == "origin"
                && eventNode.toElement().attribute("publicID") == preferredOriginID)
        {
            QDomNode originNode = eventNode.firstChild();
            while (!originNode.isNull())
            {
                if (originNode.nodeName() == "time")
                {
                    m_dateTime = QDateTime::fromString
                            (extractValueFromXmlPath(originNode, "value").split(".").at(0), "yyyy-MM-ddThh:mm:ss");
                    m_dateTime.setTimeSpec(Qt::UTC);
                }
                else if (originNode.nodeName() == "latitude")
                {
                    m_latitude = extractValueFromXmlPath(originNode, "value").toDouble();
                }
                else if (originNode.nodeName() == "longitude")
                {
                    m_longitude = extractValueFromXmlPath(originNode, "value").toDouble();
                }

                originNode = originNode.nextSibling();
            }
        }
        else if (eventNode.nodeName() == "magnitude"
                && eventNode.toElement().attribute("publicID") == preferredMagnitudeID)
        {
            QDomNode magnitudeNode = eventNode.firstChild();
            while (!magnitudeNode.isNull())
            {
                if (magnitudeNode.nodeName() == "mag")
                {
                    m_magnitude = extractValueFromXmlPath(magnitudeNode, "value").toDouble();
                }

                magnitudeNode = magnitudeNode.nextSibling();
            }
        }

        eventNode = eventNode.nextSibling();
    }
}

void CEventItem::addTraceItem(CAbstractItem *item)
{
    connect(item, SIGNAL(dataChanged(CAbstractItem *,int,int)),
            this, SIGNAL(dataChanged(CAbstractItem *,int,int)));
    connect(item, SIGNAL(rowInserted(CAbstractItem *,int,int)),
            this, SIGNAL(rowInserted(CAbstractItem *,int,int)));
    connect(item, SIGNAL(rowRemoved(CAbstractItem *,int,int)),
            this, SIGNAL(rowInserted(CAbstractItem *,int,int)));
    m_traceList.push_back(item);
}

void CEventItem::clearTraceList()
{
    int count = m_traceList.count();
    if (count > 0)
    {
        emit rowRemoved(this, 0, count-1);
        qDeleteAll(m_traceList);
        m_traceList.clear();
    }
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
