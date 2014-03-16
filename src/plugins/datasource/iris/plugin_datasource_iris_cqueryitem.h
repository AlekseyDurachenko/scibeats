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
#ifndef PLUGIN_DATASOURCE_IRIS_CQUERYTITEM_H
#define PLUGIN_DATASOURCE_IRIS_CQUERYTITEM_H

#include "plugin_datasource_iris_cabstractitem.h"
#include "plugin_datasource_iris_consts.h"
#include <QDateTime>
class QNetworkAccessManager;
class QNetworkReply;

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

class CQueryItem : public CAbstractItem
{
    Q_OBJECT
public:
    explicit CQueryItem(QNetworkAccessManager *network, QObject *parent = 0);
    inline QNetworkAccessManager *network() const;

    int childCount() const;
    CAbstractItem *child(int row);

    inline int offset() const;
    inline int limit() const;
    inline double minMagnitude() const;
    inline double maxMagnitude() const;
    inline double minRadius() const;
    inline double maxRadius() const;
    inline double minRadiusKm() const;
    inline double maxRadiusKm() const;
    inline const QDateTime &startDateTime() const;
    inline const QDateTime &endDateTime() const;

    void setOffset(int offset);
    void setLimit(int limit);
    void setMinMagnitude(double minMagnitude);
    void setMaxMagnitude(double maxMagnitude);
    void setMinRadius(double minRadius);
    void setMaxRadius(double maxRadius);
    void setMinRadiusKm(double minRadiusKm);
    void setMaxRadiusKm(double maxRadiusKm);
    void setStartDateTime(const QDateTime &startDateTime);
    void setEndDateTime(const QDateTime &endDateTime);

    void execute();

private slots:
    void slot_reply_Finished();

private:
    bool parceXml(QByteArray xml, QString *reason = 0);
    void addEventItem(CAbstractItem *item);
    void clearEventList();

private:
    QList<CAbstractItem *> m_eventList;

    QNetworkAccessManager *m_network;
    QNetworkReply *m_reply;

    int m_offset;
    int m_limit; // 0 - for all events
    double m_minMagnitude;
    double m_maxMagnitude;
    double m_minRadius; // in degree 0 .. 180
    double m_maxRadius; // in degree 0 .. 180
    QDateTime m_startDateTime;
    QDateTime m_endDateTime;

};

QNetworkAccessManager *CQueryItem::network() const
{
    return m_network;
}

inline int CQueryItem::offset() const
{
    return m_offset;
}

inline int CQueryItem::limit() const
{
    return m_limit;
}

inline double CQueryItem::minMagnitude() const
{
    return m_minMagnitude;
}

inline double CQueryItem::maxMagnitude() const
{
    return m_maxMagnitude;
}

inline double CQueryItem::minRadius() const
{
    return m_minRadius;
}

inline double CQueryItem::maxRadius() const
{
    return m_maxRadius;
}

inline double CQueryItem::minRadiusKm() const
{
    return m_minRadius * (g_earthRadiusKm / 180.0);
}

inline double CQueryItem::maxRadiusKm() const
{
    return m_maxRadius * (g_earthRadiusKm / 180.0);
}

inline const QDateTime &CQueryItem::startDateTime() const
{
    return m_startDateTime;
}

inline const QDateTime &CQueryItem::endDateTime() const
{
    return m_endDateTime;
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_CQUERYTITEM_H
