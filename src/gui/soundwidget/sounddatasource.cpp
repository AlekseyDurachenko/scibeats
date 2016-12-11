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
#include "sounddatasource.h"
#include "writablesoundpeaks.h"


SoundDataSource::SoundDataSource(QObject *parent)
    : QseAbstractSignalDataSource(parent)
{
}

void SoundDataSource::setSound(const Sound &sound)
{
    if (m_sound != sound) {
        m_sound = sound;
        // WARNING: START TEMPORARY CODE
        WritableSoundPeaks writablePeaks;
        writablePeaks.create(m_sound);
        m_peaks = writablePeaks.toSoundPeaks();
        // END TEMPORARY CODE
        emit dataChanged();
    }
}

bool SoundDataSource::isNull() const
{
    return m_sound.isNull();
}

qint64 SoundDataSource::count() const
{
    if (isNull()) {
        return 0;
    }

    return m_sound.sampleCount();
}

qint64 SoundDataSource::minIndex() const
{
    return 0;
}

qint64 SoundDataSource::maxIndex() const
{
    if (isNull()) {
        return 0;
    }

    return m_sound.sampleCount() - 1;
}

qint64 SoundDataSource::readAsPoints(double *points,
                                            qint64 index,
                                            qint64 count)
{
    if (isNull()) {
        return 0;
    }

    const qint64 sampleCount = m_sound.sampleCount();
    if (index + count > sampleCount) {
        count = sampleCount - index;
    }

    const float *data = m_sound.samples(0, index);
    for (qint64 i = 0; i < count; ++i) {
        points[i] = data[i];
    }

    return count;
}

qint64 SoundDataSource::readAsPeaks(double *minimums,
                                           double *maximums,
                                           qint64 index,
                                           qint64 spp,
                                           qint64 count)
{
    if (isNull()) {
        return 0;
    }

    const qint64 total = m_peaks.count(spp, index);
    if (count > total) {
        count = total;
    }

    const float *dataMin = m_peaks.minimums(0, spp, index);
    const float *dataMax = m_peaks.maximums(0, spp, index);
    for (int i = 0; i < count; ++i) {
        minimums[i] = dataMin[i];
        maximums[i] = dataMax[i];
    }

    return count;
}
