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
#include "soundpeaksdata.h"
#include "consts.h"
#include <limits>
#include <iostream>


SoundPeaksData::SoundPeaksData()
{
    file = new QTemporaryFile;
    file->setFileTemplate(appName());
    file->setAutoRemove(true);
    peaks = nullptr;
}

SoundPeaksData::~SoundPeaksData()
{
    delete file;
}

qint64 SoundPeaksData::calcPeakCount(qint64 spp, qint64 firstSample) const
{
    Q_ASSERT(spp > 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(peakCount.contains(spp));

    const qint64 count = peakCount[spp] - firstSample / spp;
    if (count < 0) {
        return 0;
    }

    return count;
}

qint64 SoundPeaksData::calcMinPeakIndex(int channel, qint64 spp, qint64 firstSample) const
{
    Q_ASSERT(channel >= 0);
    Q_ASSERT(spp > 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(minOffset.contains(channel));
    Q_ASSERT(minOffset[channel].contains(spp));

    return minOffset[channel][spp] + firstSample / spp;
}

qint64 SoundPeaksData::calcMaxPeakIndex(int channel, qint64 spp, qint64 firstSample) const
{
    Q_ASSERT(channel >= 0);
    Q_ASSERT(spp > 0);
    Q_ASSERT(firstSample >= 0);
    Q_ASSERT(maxOffset.contains(channel));
    Q_ASSERT(maxOffset[channel].contains(spp));

    return maxOffset[channel][spp] + firstSample / spp;
}
