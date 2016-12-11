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
#ifndef SOUNDPEAKSDATA_H
#define SOUNDPEAKSDATA_H


#include "version.h"
#include "sound.h"
#include <QTemporaryFile>
#include <QSharedData>
#include <QHash>


class SoundPeaksData : public QSharedData
{
public:
    SoundPeaksData();
    virtual ~SoundPeaksData() final;

    qint64 calcPeakCount(qint64 spp, qint64 firstSample = 0) const;
    qint64 calcMinPeakIndex(int channel, qint64 spp, qint64 firstSample = 0) const;
    qint64 calcMaxPeakIndex(int channel, qint64 spp, qint64 firstSample = 0) const;

public:
    QTemporaryFile *file;
    // structure of the peaks:
    // CH_0_SPP_2_MIN
    // CH_0_SPP_2_MAX
    // CH_1_SPP_2_MIN
    // CH_1_SPP_2_MAX
    // CH_0_SPP_4_MIN
    // CH_0_SPP_4_MAX
    // CH_1_SPP_4_MIN
    // CH_1_SPP_4_MAX
    // ..
    // CH_0_SPP_N_MIN
    // CH_0_SPP_N_MAX
    // CH_1_SPP_N_MIN
    // CH_1_SPP_N_MAX
    float *peaks;
    //  channel       spp     addr
    QHash<qint64, qint64> peakCount;
    QHash<int, QHash<qint64, qint64>> maxOffset;
    QHash<int, QHash<qint64, qint64>> minOffset;
};


#endif // SOUNDPEAKSDATA_H
