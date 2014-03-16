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
#include "plugin_datasource_iris_ctraceitem.h"
#include "plugin_datasource_iris_ceventitem.h"
#include "plugin_datasource_iris_utils.h"
#include "plugin_datasource_iris_consts.h"
#include <QNetworkAccessManager>
#include <QIcon>
#include <QDebug>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CTraceItem::CTraceItem(const QDomElement& traceElem, const QString &fdsn, const QString &station,
        const QString &siteName, int row, QObject *parent) : CAbstractItem(Trace, row, parent)
{
    m_latitude  = 0.0;
    m_longitude = 0.0;
    m_sampleRate = 20.0;
    m_station = station;
    m_fdsn = fdsn;
    m_siteName = siteName;
    setXml(traceElem);

    m_dataFound = true;
}

QVariant CTraceItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
        {
            return QString("%1.%2.%3.%4").arg(m_fdsn, m_station, m_location.trimmed(), m_channel);
        }        
        else if (column == 1)
        {
            return QString::number(distance() / 1000.0, 'f', 0) + tr(" km");
        }
        else if (column == 2)
        {
            return phaseDateTime().toString("yyyy-MM-ddTHH:mm:ss");
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
        if (m_dataFound)
        {
            return QIcon(":/plugin/datasource/iris/icons/item-trace.png");
        }
        else
        {
            return QIcon(":/plugin/datasource/iris/icons/status-notfound.png");
        }
    }

    return QVariant();
}

void CTraceItem::setDataFound(bool found)
{
    if (m_dataFound != found)
    {
        m_dataFound = found;
        emit dataChanged(eventItem(), row(), row());
    }
}

// The xml structure example:
//
//<Channel locationCode="00" startDate="2011-11-01T00:00:00" restrictedStatus="open" endDate="2599-12-31T23:59:59" code="BHE">
//  <Comment>
//    <Value>Mass centering in progress</Value>
//    <BeginEffectiveTime>2013-05-13T09:22:00</BeginEffectiveTime>
//    <EndEffectiveTime>2013-05-13T09:30:00</EndEffectiveTime>
//  </Comment>
//  <Comment>
//    <Value>Mass centering in progress</Value>
//    <BeginEffectiveTime>2013-06-04T08:20:00</BeginEffectiveTime>
//    <EndEffectiveTime>2013-06-04T08:30:00</EndEffectiveTime>
//  </Comment>
//  <Latitude>-15.447148</Latitude>
//  <Longitude>167.203231</Longitude>
//  <Elevation>56.0</Elevation>
//  <Depth>0.0</Depth>
//  <Azimuth>90.0</Azimuth>
//  <Dip>0.0</Dip>
//  <Type>CONTINUOUS</Type>
//  <Type>GEOPHYSICAL</Type>
//  <SampleRate>20.0</SampleRate>
//  <ClockDrift>5.0E-6</ClockDrift>
//  <Sensor>
//    <Type>STRECKEISEN STS2</Type>
//  </Sensor>
//  <Response>
//    <InstrumentSensitivity>
//      <Value>2.51661E9</Value>
//      <Frequency>0.02</Frequency>
//      <InputUnits>
//	<Name>M/S</Name>
//	<Description>Velocity</Description>
//      </InputUnits>
//      <OutputUnits>
//	<Name>COUNTS</Name>
//	<Description>Digital Counts</Description>
//      </OutputUnits>
//    </InstrumentSensitivity>
//  </Response>
//</Channel>
void CTraceItem::setXml(const QDomElement &channelElem)
{
    m_channel = channelElem.attribute("code");
    m_location = channelElem.attribute("locationCode");


    QDomNode channelNode = channelElem.firstChild();
    while (!channelNode.isNull())
    {
        if (channelNode.nodeName() == "Latitude")
        {
            m_latitude = channelNode.toElement().text().toDouble();
        }
        else if (channelNode.nodeName() == "Longitude")
        {
            m_longitude = channelNode.toElement().text().toDouble();
        }
        else if (channelNode.nodeName() == "SampleRate")
        {
            m_sampleRate = channelNode.toElement().text().toDouble();
        }

        channelNode = channelNode.nextSibling();
    }
}

double CTraceItem::distance() const
{
    return calcDistance(m_latitude, m_longitude, eventItem()->latitude(), eventItem()->longitude());
}

QDateTime CTraceItem::phaseDateTime() const
{
    // we use the travel-time digaramm for calculate the estimated first phase time.
    // 20min (1200sec) is maximum seismic travel time for first phase, see the
    // http://earthquake.usgs.gov/learn/topics/ttgraph.php
    return eventItem()->dateTime().addSecs(1200/g_earthRadiusKm*(distance()/1000.0));
}

CEventItem *CTraceItem::eventItem() const
{
    return qobject_cast<CEventItem *>(parent());
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
