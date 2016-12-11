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
#ifndef SOUNDPEAKS_H
#define SOUNDPEAKS_H


#include "soundpeaksdata.h"
#include <QSharedDataPointer>
#include <QDebug>


class SoundPeaks
{
    friend class WritableSoundPeaks;
private:
    SoundPeaks(SoundPeaksData *soundPeaksData);

public:
    SoundPeaks();
    SoundPeaks(const SoundPeaks &other);

    inline bool isNull() const;

    inline qint64 count(qint64 spp, qint64 firstSample = 0) const;
    inline const float *minimums(int channel, qint64 spp, qint64 firstSample = 0) const;
    inline const float *maximums(int channel, qint64 spp, qint64 firstSample = 0) const;

    inline bool operator == (const SoundPeaks &other) const;
    inline bool operator != (const SoundPeaks &other) const;

private:
    QExplicitlySharedDataPointer<SoundPeaksData> data;
};
Q_DECLARE_METATYPE(SoundPeaks)


bool SoundPeaks::isNull() const
{
    return !data || !data->peaks;
}

qint64 SoundPeaks::count(qint64 spp, qint64 firstSample) const
{
    Q_ASSERT(spp > 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->peaks != nullptr);

    return data->calcPeakCount(spp, firstSample);
}

const float *SoundPeaks::minimums(int channel, qint64 spp, qint64 firstSample) const
{
    Q_ASSERT(channel >= 0);
    Q_ASSERT(spp > 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->peaks != nullptr);

    return &data->peaks[data->calcMinPeakIndex(channel, spp, firstSample)];
}

const float *SoundPeaks::maximums(int channel, qint64 spp, qint64 firstSample) const
{
    Q_ASSERT(channel >= 0);
    Q_ASSERT(spp > 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(data != nullptr);
    Q_ASSERT(data->peaks != nullptr);

    return &data->peaks[data->calcMaxPeakIndex(channel, spp, firstSample)];
}

bool SoundPeaks::operator ==(const SoundPeaks &other) const
{
    return data == other.data;
}

bool SoundPeaks::operator !=(const SoundPeaks &other) const
{
    return data != other.data;
}


#endif // SOUNDPEAKS_H
