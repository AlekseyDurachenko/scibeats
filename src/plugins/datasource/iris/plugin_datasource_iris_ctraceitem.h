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
#ifndef PLUGIN_DATASOURCE_IRIS_CTRACEITEM_H
#define PLUGIN_DATASOURCE_IRIS_CTRACEITEM_H

#include "plugin_datasource_iris_cabstractitem.h"
#include <QDateTime>
#include <QDomElement>
class QNetworkAccessManager;

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

class CEventItem;
class CTraceItem : public CAbstractItem
{
    Q_OBJECT
public:
    CTraceItem(const QDomElement& traceElem, const QString &fdsn, const QString &station,
            const QString &siteName, int row, QObject *parent = 0);

    QVariant data(int column, int role) const;

    inline bool isDataFound() const;
    void setDataFound(bool found);

    CEventItem *eventItem() const;

    inline const QString &fdsn() const;
    inline const QString &station() const;
    inline const QString &channel() const;
    inline const QString &location() const;
    inline double latitude() const;
    inline double longitude() const;
    inline double sampleRate() const;
    inline const QString &siteName() const;
    double distance() const;
    QDateTime phaseDateTime() const;

private:
    void setXml(const QDomElement &eventElem);

private:
    QString m_fdsn;
    QString m_station;
    QString m_channel;
    QString m_location;
    double m_latitude;
    double m_longitude;
    double m_sampleRate;
    QString m_siteName;
    bool m_dataFound;

};

bool CTraceItem::isDataFound() const
{
    return m_dataFound;
}

const QString &CTraceItem::fdsn() const
{
    return m_fdsn;
}

const QString &CTraceItem::station() const
{
    return m_station;
}

const QString &CTraceItem::channel() const
{
    return m_channel;
}

const QString &CTraceItem::location() const
{
    return m_location;
}

double CTraceItem::latitude() const
{
    return m_latitude;
}

double CTraceItem::longitude() const
{
    return m_longitude;
}

double CTraceItem::sampleRate() const
{
    return m_sampleRate;
}

const QString &CTraceItem::siteName() const
{
    return m_siteName;
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_CTRACEITEM_H
