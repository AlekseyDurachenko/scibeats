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
#ifndef PLUGIN_DATASOURCE_IRIS_CEVENTITEM_H
#define PLUGIN_DATASOURCE_IRIS_CEVENTITEM_H

#include "plugin_datasource_iris_cabstractitem.h"
#include <QDateTime>
#include <QDomElement>
class QNetworkAccessManager;
class QNetworkReply;

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

class CQueryItem;
class CEventItem : public CAbstractItem
{
    Q_OBJECT
public:
    CEventItem(const QDomElement& eventElem, int row, QObject *parent = 0);

    inline int eventId() const;
    inline double magnitude() const;
    inline const QDateTime &dateTime() const;
    inline double latitude() const;
    inline double longitude() const;
    inline const QString &siteName() const;

    int childCount() const;
    QVariant data(int column, int role) const;
    CAbstractItem *child(int row);

private slots:
    void slot_reply_Finished();

private:
    CQueryItem *queryItem() const;
    bool parceXml(QByteArray xml, QString *reason = 0);
    void setXml(const QDomElement& eventElem);
    void addTraceItem(CAbstractItem *item);
    void clearTraceList();

private:
    QList<CAbstractItem *> m_traceList;
    QNetworkReply *m_reply;
    bool m_loaded;

    int m_eventId;
    double m_magnitude;
    QDateTime m_dateTime;
    double m_latitude;
    double m_longitude;
    QString m_siteName;
};

int CEventItem::eventId() const
{
    return m_eventId;
}

inline double CEventItem::magnitude() const
{
    return m_magnitude;
}

inline const QDateTime &CEventItem::dateTime() const
{
    return m_dateTime;
}

inline double CEventItem::latitude() const
{
    return m_latitude;
}

inline double CEventItem::longitude() const
{
    return m_longitude;
}

inline const QString &CEventItem::siteName() const
{
    return m_siteName;
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_CEVENTITEM_H
