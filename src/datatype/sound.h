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
#ifndef SOUND_H
#define SOUND_H


#include "sounddata.h"
#include <QSharedDataPointer>
#include <QMetaType>


class Sound
{
    friend class WritableSound;
private:
    Sound(SoundData *soundData);

public:
    Sound();
    Sound(const Sound &other);

    inline bool isNull() const;

    inline int channelCount() const;
    inline qint64 sampleCount() const;

    inline const float *samples(int channel = 0, qint64 firstSample = 0) const;

    inline bool operator == (const Sound &other) const;
    inline bool operator != (const Sound &other) const;

private:
    QExplicitlySharedDataPointer<SoundData> data;
};
Q_DECLARE_METATYPE(Sound)


bool Sound::isNull() const
{
    return !data || !data->samples;
}

int Sound::channelCount() const
{
    Q_ASSERT(data != nullptr);

    return data->channelCount;
}

qint64 Sound::sampleCount() const
{
    Q_ASSERT(data != nullptr);

    return data->sampleCount;
}

const float *Sound::samples(int channel, qint64 firstSample) const
{
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->samples != nullptr);

    return &data->samples[data->calcSamplesIndex(channel, firstSample)];
}

bool Sound::operator ==(const Sound &other) const
{
    return data == other.data;
}

bool Sound::operator !=(const Sound &other) const
{
    return data != other.data;
}


#endif // SOUND_H
