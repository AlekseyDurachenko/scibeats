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
#include "test_sounddata.h"
#include "sounddata.h"
#include "consts.h"


Test_SoundData::Test_SoundData(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundData::test_constructor()
{
    SoundData soundData;
    QVERIFY(soundData.file != nullptr);
    QVERIFY(soundData.samples == nullptr);
    QCOMPARE(soundData.channelCount, static_cast<int>(0));
    QCOMPARE(soundData.sampleCount, static_cast<qint64>(0));
}

void Test_SoundData::test_calcSamplesOffset()
{
    SoundData soundData;
    soundData.channelCount = 2;
    soundData.sampleCount = 100;
    QCOMPARE(soundData.calcSamplesIndex(0, 0), static_cast<qint64>(0));
    QCOMPARE(soundData.calcSamplesIndex(0, 10), static_cast<qint64>(10));
    QCOMPARE(soundData.calcSamplesIndex(1, 0), static_cast<qint64>(100));
    QCOMPARE(soundData.calcSamplesIndex(1, 20), static_cast<qint64>(120));
    QCOMPARE(soundData.calcSamplesIndex(1, 99), static_cast<qint64>(199));
}
