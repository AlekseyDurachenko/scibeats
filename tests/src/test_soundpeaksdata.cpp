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
#include "test_soundpeaksdata.h"
#include "soundpeaksdata.h"
#include "consts.h"


Test_SoundPeaksData::Test_SoundPeaksData(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundPeaksData::test_constructor()
{
    SoundPeaksData soundData;
    QVERIFY(soundData.file != nullptr);
    QVERIFY(soundData.peaks == nullptr);
    QVERIFY(soundData.peakCount.isEmpty());
    QVERIFY(soundData.maxOffset.isEmpty());
    QVERIFY(soundData.minOffset.isEmpty());
}

void Test_SoundPeaksData::test_calcPeakCount()
{
    SoundPeaksData soundPeakData;
    soundPeakData.peakCount[2] = 10;
    soundPeakData.peakCount[4] = 30;
    soundPeakData.peakCount[8] = 50;


    QCOMPARE(soundPeakData.calcPeakCount(2, 0), static_cast<qint64>(10));
    QCOMPARE(soundPeakData.calcPeakCount(2, 1), static_cast<qint64>(10));
    QCOMPARE(soundPeakData.calcPeakCount(2, 2), static_cast<qint64>(9));
    QCOMPARE(soundPeakData.calcPeakCount(2, 10), static_cast<qint64>(5));
    QCOMPARE(soundPeakData.calcPeakCount(2, 11), static_cast<qint64>(5));
    QCOMPARE(soundPeakData.calcPeakCount(2, 20), static_cast<qint64>(0));
    QCOMPARE(soundPeakData.calcPeakCount(2, 30), static_cast<qint64>(0));
}

void Test_SoundPeaksData::test_calcMinPeakIndex()
{
    SoundPeaksData soundPeakData;
    soundPeakData.minOffset[0] = QHash<qint64, qint64>();
    soundPeakData.minOffset[0][2] = 10;
    soundPeakData.minOffset[0][4] = 30;
    soundPeakData.minOffset[0][8] = 50;
    soundPeakData.minOffset[1] = QHash<qint64, qint64>();
    soundPeakData.minOffset[1][2] = 20;
    soundPeakData.minOffset[1][4] = 40;
    soundPeakData.minOffset[1][8] = 60;

    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 0), static_cast<qint64>(10));
    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 1), static_cast<qint64>(10));
    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 2), static_cast<qint64>(11));
    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 10), static_cast<qint64>(15));
    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 11), static_cast<qint64>(15));
    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 20), static_cast<qint64>(20));
    QCOMPARE(soundPeakData.calcMinPeakIndex(0, 2, 30), static_cast<qint64>(25));

    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 0), static_cast<qint64>(40));
    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 1), static_cast<qint64>(40));
    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 4), static_cast<qint64>(41));
    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 10), static_cast<qint64>(42));
    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 12), static_cast<qint64>(43));
    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 20), static_cast<qint64>(45));
    QCOMPARE(soundPeakData.calcMinPeakIndex(1, 4, 30), static_cast<qint64>(47));
}

void Test_SoundPeaksData::test_calcMaxPeakIndex()
{
    SoundPeaksData soundPeakData;
    soundPeakData.maxOffset[0] = QHash<qint64, qint64>();
    soundPeakData.maxOffset[0][2] = 10;
    soundPeakData.maxOffset[0][4] = 30;
    soundPeakData.maxOffset[0][8] = 50;
    soundPeakData.maxOffset[1] = QHash<qint64, qint64>();
    soundPeakData.maxOffset[1][2] = 20;
    soundPeakData.maxOffset[1][4] = 40;
    soundPeakData.maxOffset[1][8] = 60;

    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 0), static_cast<qint64>(10));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 1), static_cast<qint64>(10));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 2), static_cast<qint64>(11));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 10), static_cast<qint64>(15));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 11), static_cast<qint64>(15));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 20), static_cast<qint64>(20));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(0, 2, 30), static_cast<qint64>(25));

    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 0), static_cast<qint64>(40));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 1), static_cast<qint64>(40));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 4), static_cast<qint64>(41));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 10), static_cast<qint64>(42));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 12), static_cast<qint64>(43));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 20), static_cast<qint64>(45));
    QCOMPARE(soundPeakData.calcMaxPeakIndex(1, 4, 30), static_cast<qint64>(47));
}
