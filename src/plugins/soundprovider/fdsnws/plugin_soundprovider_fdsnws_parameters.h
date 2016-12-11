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
#ifndef PLUGIN_SOUNDPROVIDER_FDSNWS_PARAMETERS_H
#define PLUGIN_SOUNDPROVIDER_FDSNWS_PARAMETERS_H


#include "soundtags.h"
#include <QUrl>
#include <QString>
#include <QDateTime>


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
namespace fdsnws {
// ----------------------------------------------------------------------


class Parameters
{
public:
    Parameters();

    inline const QUrl &url() const;
    void setUrl(const QUrl &url);

    inline const QString &network() const;
    void setNetwork(const QString &network);

    inline const QString &station() const;
    void setStation(const QString &station);

    inline const QString &location() const;
    void setLocation(const QString &location);

    inline const QString &channel() const;
    void setChannel(const QString &channel);

    inline const QDateTime &startTime() const;
    void setStartTime(const QDateTime &startTime);

    inline const QDateTime &endTime() const;
    void setEndTime(const QDateTime &endTime);

    inline const SoundTags &tags() const;
    void setTags(const SoundTags &tags);

    bool fromXml(const QByteArray &xml, QString *reason = 0);
    QByteArray toXml() const;

    QUrl buildQueryUrl() const;

public:
    static bool validate(const QByteArray &xml, QString *reason = 0);

private:
    QUrl m_url;
    QString m_network;
    QString m_station;
    QString m_location;
    QString m_channel;
    QDateTime m_startTime;
    QDateTime m_endTime;
    SoundTags m_tags;
};


const QUrl &Parameters::url() const
{
    return m_url;
}

const QString &Parameters::network() const
{
    return m_network;
}

const QString &Parameters::station() const
{
    return m_station;
}

const QString &Parameters::location() const
{
    return m_location;
}

const QString &Parameters::channel() const
{
    return m_channel;
}

const QDateTime &Parameters::startTime() const
{
    return m_startTime;
}

const QDateTime &Parameters::endTime() const
{
    return m_endTime;
}

const SoundTags &Parameters::tags() const
{
    return m_tags;
}


// ----------------------------------------------------------------------
} // namespace fdsnws
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------


#endif // PLUGIN_SOUNDPROVIDER_FDSNWS_PARAMETERS_H
