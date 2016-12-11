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
#ifndef SOUNDPROVIDERREQUEST_H
#define SOUNDPROVIDERREQUEST_H


#include <QMetaType>


class SoundProviderRequest
{
public:
    SoundProviderRequest();

    inline bool isNull() const;

    inline const QByteArray &query() const;
    void setQuery(const QByteArray &query);

    inline bool operator ==(const SoundProviderRequest &other) const;
    inline bool operator !=(const SoundProviderRequest &other) const;
private:
    QByteArray m_query;
};
Q_DECLARE_METATYPE(SoundProviderRequest)


bool SoundProviderRequest::isNull() const
{
    return m_query.isNull();
}

const QByteArray &SoundProviderRequest::query() const
{
    return m_query;
}

bool SoundProviderRequest::operator ==(const SoundProviderRequest &other) const
{
    return m_query == other.m_query;
}

bool SoundProviderRequest::operator !=(const SoundProviderRequest &other) const
{
    return !operator==(other);
}


#endif // SOUNDPROVIDERREQUEST_H
