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
#ifndef WRITABLESOUND_H
#define WRITABLESOUND_H


#include "sound.h"
#include "sounddata.h"
#include <QSharedDataPointer>


class WritableSound
{
public:
    WritableSound();
    ~WritableSound();

    inline bool isNull() const;

    inline int channelCount() const;
    inline qint64 sampleCount() const;

    inline float *samples(int channel = 0, qint64 firstSample = 0);
    inline const float *samples(int channel = 0, qint64 firstSample = 0) const;

    bool create(int channelCount, qint64 sampleCount, QString *reason = 0);
    void clear();

    bool fromSound(const Sound &sound, QString *reason = 0);
    Sound toSound();

public:
    WritableSound(const WritableSound &other) = delete;
    WritableSound& operator=(WritableSound const&) = delete;

private:
    SoundData *data;
};


bool WritableSound::isNull() const
{
    return !data || !data->samples;
}

int WritableSound::channelCount() const
{
    Q_ASSERT(data != nullptr);

    return data->channelCount;
}

qint64 WritableSound::sampleCount() const
{
    Q_ASSERT(data != nullptr);

    return data->sampleCount;
}

float *WritableSound::samples(int channel, qint64 firstSample)
{
    Q_ASSERT(channel >= 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->samples != nullptr);

    return &data->samples[data->calcSamplesIndex(channel, firstSample)];
}

const float *WritableSound::samples(int channel, qint64 firstSample) const
{
    return samples(channel, firstSample);
}


#endif // WRITABLESOUND_H
