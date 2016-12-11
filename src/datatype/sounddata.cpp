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
#include "sounddata.h"
#include "consts.h"
#include <limits>
#include <iostream>


SoundData::SoundData()
{
    file = new QTemporaryFile;
    file->setFileTemplate(appName());
    file->setAutoRemove(true);
    samples = nullptr;
    channelCount = 0;
    sampleCount = 0;
}

SoundData::~SoundData()
{
    delete file;
}

qint64 SoundData::calcSamplesIndex(int channel, qint64 firstSample) const
{
    Q_ASSERT(channel >= 0);
    Q_ASSERT(channel < channelCount);
    Q_ASSERT(firstSample >= 0);

    const qint64 offset = sampleCount * channel + firstSample;
    Q_ASSERT(offset < sampleCount * channelCount);

    return offset;
}
