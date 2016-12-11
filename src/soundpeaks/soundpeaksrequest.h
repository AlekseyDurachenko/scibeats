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
#ifndef SOUNDPEAKSREQUEST_H
#define SOUNDPEAKSREQUEST_H


#include "sound.h"
#include "soundpeaks.h"
#include <QMetaType>


class SoundPeaksRequest
{
public:
    SoundPeaksRequest();

    inline bool isNull() const;

    inline const Sound &sound() const;
    void setSound(const Sound &sound);

    inline bool operator ==(const SoundPeaksRequest &other) const;
    inline bool operator !=(const SoundPeaksRequest &other) const;
private:
    Sound m_sound;
};
Q_DECLARE_METATYPE(SoundPeaksRequest)


bool SoundPeaksRequest::isNull() const
{
    return m_sound.isNull();
}

const Sound &SoundPeaksRequest::sound() const
{
    return m_sound;
}

bool SoundPeaksRequest::operator ==(const SoundPeaksRequest &other) const
{
    return m_sound == other.m_sound;
}

bool SoundPeaksRequest::operator !=(const SoundPeaksRequest &other) const
{
    return !operator==(other);
}


#endif // SOUNDPEAKSREQUEST_H
