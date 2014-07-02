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
#include "plugin_datasource_iris_cqueryitem.h"
#include "plugin_datasource_iris_ceventitem.h"
#include "plugin_datasource_iris_cprogressitem.h"
#include "plugin_datasource_iris_cnotfounditem.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QtXml>
#include <QDebug>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CQueryItem::CQueryItem(QNetworkAccessManager *network, QObject *parent) :
    CAbstractItem(Query, 0, parent)
{
    m_network = network;
    m_reply = 0;

    m_offset = 0;
    m_limit = 0;
    m_minMagnitude = 8.0;
    m_maxMagnitude = 9.9;
    m_minRadius = 0.0;
    m_maxRadius = 180.0;
    m_startDateTime = QDateTime(QDate(2000, 0, 0), QTime(0, 0, 0), Qt::UTC);
    m_endDateTime = QDateTime::currentDateTime();
}

int CQueryItem::childCount() const
{
    return m_eventList.count();
}

CAbstractItem *CQueryItem::child(int index)
{
    return m_eventList[index];
}

void CQueryItem::setOffset(int offset)
{
    m_offset = offset;
}

void CQueryItem::setLimit(int limit)
{
    m_limit = limit;
}

void CQueryItem::setMinMagnitude(double minMagnitude)
{
    m_minMagnitude = minMagnitude;
}

void CQueryItem::setMaxMagnitude(double maxMagnitude)
{
    m_maxMagnitude = maxMagnitude;
}

void CQueryItem::setMinRadius(double minRadius)
{
    m_minRadius = minRadius;
}

void CQueryItem::setMaxRadius(double maxRadius)
{
    m_maxRadius = maxRadius;
}

void CQueryItem::setMinRadiusKm(double minRadiusKm)
{
    m_minRadius = (minRadiusKm / g_earthRadiusKm) * 180.0;
}

void CQueryItem::setMaxRadiusKm(double maxRadiusKm)
{
    m_maxRadius = (maxRadiusKm / g_earthRadiusKm) * 180.0;
}

void CQueryItem::setStartDateTime(const QDateTime &startDateTime)
{
    m_startDateTime = startDateTime;
}

void CQueryItem::setEndDateTime(const QDateTime &endDateTime)
{
    m_endDateTime = endDateTime;
}

void CQueryItem::execute()
{
    // cancel previouse query
    if (m_reply) m_reply->abort();
    clearEventList();

    // create new query
    QUrl queryUrl("http://service.iris.edu/fdsnws/event/1/query");
    queryUrl.addQueryItem("format", "xml");
    queryUrl.addQueryItem("nodata", "404");
    queryUrl.addQueryItem("minmagnitude", QString::number(m_minMagnitude, 'f', 1));
    queryUrl.addQueryItem("maxmagnitude", QString::number(m_maxMagnitude, 'f', 1));
    queryUrl.addQueryItem("starttime", m_startDateTime.toString("yyyy-MM-ddTHH:mm:ss"));
    queryUrl.addQueryItem("endtime", m_endDateTime.toString("yyyy-MM-ddTHH:mm:ss"));
    //queryUrl.addQueryItem("offset", QString::number(m_offset));
    if (m_limit > 0) queryUrl.addQueryItem("limit", QString::number(m_limit));

    // threre should be log output
    qDebug() << queryUrl.toString();

    // show progress bar
    CProgressItem *progressItem = new CProgressItem(0, this);
    m_reply = m_network->get(QNetworkRequest(queryUrl));
    connect(m_reply, SIGNAL(finished()), this, SLOT(slot_reply_Finished()));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), progressItem, SLOT(setProgress(qint64,qint64)));
    addEventItem(progressItem);
    emit rowInserted(this, 0, 0);
}

void CQueryItem::slot_reply_Finished()
{
    clearEventList();

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
        if (m_eventList.count() == 0)
        {
            addEventItem(new CNotFoundItem(0, this));
        }
        emit rowInserted(this, 0, m_eventList.count());
        break;
    }
    default:
        break;
    }

    m_reply->deleteLater();
    m_reply = 0;
}

// The xml structure example:
//
//<q:quakeml xmlns:q="http://quakeml.org/xmlns/quakeml/1.2" xmlns:iris="http://service.iris.edu/fdsnws/event/1/" xmlns="http://quakeml.org/xmlns/bed/1.2" xmlns:xsi="http://www.w3.org/2000/10/XMLSchema-instance" xsi:schemaLocation="http://quakeml.org/schema/xsd http://quakeml.org/schema/xsd/QuakeML-1.2.xsd">
//  <eventParameters publicID="smi:service.iris.edu/fdsnws/event/1/query">
//    <event xmlns:iris="http://service.iris.edu/fdsnws/event/1/" publicID="smi:service.iris.edu/fdsnws/event/1/query?eventid=4370930">
//    ...
//    </event>
//    ...
//  </eventParameters>
//</q:quakeml>
bool CQueryItem::parceXml(QByteArray xml, QString *reason)
{
    try
    {
        QDomDocument doc("q:quakeml");
        if (!doc.setContent(xml))
        {
            throw tr("Unsupported XML format", "CQueryItem::parceXml");
        }

        // we cannot parce the unknow xml format
        QDomElement rootElem = doc.documentElement();
        if (rootElem.nodeName() != "q:quakeml")
        {
            throw tr("Unsupported XML format", "CQueryItem::parceXml");
        }

        int index = -1;
        QDomNode eventParametersNode = rootElem.firstChild();
        while (!eventParametersNode.isNull())
        {
            if (eventParametersNode.nodeName() == "eventParameters")
            {
                QDomNode eventNode = eventParametersNode.firstChild();
                while (!eventNode.isNull())
                {
                    if (eventNode.nodeName() == "event")
                    {
                        addEventItem(new CEventItem(eventNode.toElement(), ++index, this));
                    }

                    eventNode = eventNode.nextSibling();
                }
            }

            eventParametersNode = eventParametersNode.nextSibling();
        }

        return true;
    }
    catch (const QString &error)
    {
        if (reason) *reason = error;
    }

    return false;
}

void CQueryItem::addEventItem(CAbstractItem *item)
{
    connect(item, SIGNAL(dataChanged(CAbstractItem *,int,int)),
            this, SIGNAL(dataChanged(CAbstractItem *,int,int)));
    connect(item, SIGNAL(rowInserted(CAbstractItem *,int,int)),
            this, SIGNAL(rowInserted(CAbstractItem *,int,int)));
    connect(item, SIGNAL(rowRemoved(CAbstractItem *,int,int)),
            this, SIGNAL(rowInserted(CAbstractItem *,int,int)));
    connect(item, SIGNAL(errorHappend(CAbstractItem*,QString)),
            this, SIGNAL(errorHappend(CAbstractItem*,QString)));
    m_eventList.push_back(item);
}

void CQueryItem::clearEventList()
{
    int count = m_eventList.count();
    if (count > 0)
    {
        emit rowRemoved(this, 0, count-1);
        qDeleteAll(m_eventList);
        m_eventList.clear();
    }
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
