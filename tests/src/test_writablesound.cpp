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
#include "test_writablesound.h"
#include "writablesound.h"


Test_WritableSound::Test_WritableSound(QObject *parent)
    : QObject(parent)
{
}

void Test_WritableSound::test_defaultConstructor()
{
    WritableSound sound;
    QCOMPARE(sound.isNull(), true);
}

void Test_WritableSound::test_createAndClear()
{
    WritableSound sound;
    // test channel count
    QCOMPARE(sound.create(0, 10), false);
    QCOMPARE(sound.create(1, 10), true);
    QCOMPARE(sound.create(2, 10), true);
    QCOMPARE(sound.create(3, 10), false);
    // test sample count
    QCOMPARE(sound.create(1, 0), false);
    QCOMPARE(sound.create(1, 1), true);
    QCOMPARE(sound.create(1, 100), true);
    // test values after creation
    QCOMPARE(sound.isNull(), false);
    QCOMPARE(sound.channelCount(), static_cast<int>(1));
    QCOMPARE(sound.sampleCount(), static_cast<qint64>(100));
    QVERIFY(sound.samples() != nullptr);
    // test clear
    sound.clear();
    QCOMPARE(sound.isNull(), true);
}

void Test_WritableSound::test_samples()
{
    WritableSound sound;

    // test 1(one) channel
    QCOMPARE(sound.create(1, 10), true);
    // test (01)
    float *samples_1_01 = sound.samples();
    samples_1_01[0] = 10.0;
    samples_1_01[1] = 11.0;
    samples_1_01[2] = 12.0;
    samples_1_01[3] = 13.0;
    samples_1_01[4] = 14.0;
    samples_1_01[5] = 15.0;
    samples_1_01[6] = 16.0;
    samples_1_01[7] = 17.0;
    samples_1_01[8] = 18.0;
    samples_1_01[9] = 19.0;
    QVERIFY(qFuzzyCompare(samples_1_01[0], 10.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[1], 11.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[2], 12.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[3], 13.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[4], 14.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[5], 15.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[6], 16.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[7], 17.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[8], 18.0f));
    QVERIFY(qFuzzyCompare(samples_1_01[9], 19.0f));
    // test (02)
    float *samples_1_02 = sound.samples(0, 5);
    QVERIFY(qFuzzyCompare(samples_1_02[0], 15.0f));
    QVERIFY(qFuzzyCompare(samples_1_02[1], 16.0f));
    QVERIFY(qFuzzyCompare(samples_1_02[2], 17.0f));
    QVERIFY(qFuzzyCompare(samples_1_02[3], 18.0f));
    QVERIFY(qFuzzyCompare(samples_1_02[4], 19.0f));

    // test 2(two) channels
    QCOMPARE(sound.create(2, 5), true);
    // test (01)
    float *samples_2_01 = sound.samples();
    samples_2_01[0] = 10.0;
    samples_2_01[1] = 11.0;
    samples_2_01[2] = 12.0;
    samples_2_01[3] = 13.0;
    samples_2_01[4] = 14.0;
    samples_2_01[5] = 15.0;
    samples_2_01[6] = 16.0;
    samples_2_01[7] = 17.0;
    samples_2_01[8] = 18.0;
    samples_2_01[9] = 19.0;
    QVERIFY(qFuzzyCompare(samples_2_01[0], 10.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[1], 11.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[2], 12.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[3], 13.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[4], 14.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[5], 15.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[6], 16.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[7], 17.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[8], 18.0f));
    QVERIFY(qFuzzyCompare(samples_2_01[9], 19.0f));
    // test (02)
    float *samples_2_02 = sound.samples(0, 2);
    QVERIFY(qFuzzyCompare(samples_2_02[0], 12.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[1], 13.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[2], 14.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[3], 15.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[4], 16.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[5], 17.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[6], 18.0f));
    QVERIFY(qFuzzyCompare(samples_2_02[7], 19.0f));
    // test (03)
    float *samples_2_03 = sound.samples(1, 2);
    QVERIFY(qFuzzyCompare(samples_2_03[0], 17.0f));
    QVERIFY(qFuzzyCompare(samples_2_03[1], 18.0f));
    QVERIFY(qFuzzyCompare(samples_2_03[2], 19.0f));
}

void Test_WritableSound::test_fromToSound()
{
    // create first writable sound
    WritableSound writableSound_01;
    QCOMPARE(writableSound_01.create(1, 5), true);
    float *writableSamples_01 = writableSound_01.samples();
    writableSamples_01[0] = 101.0;
    writableSamples_01[1] = 101.0;
    writableSamples_01[2] = 101.0;
    writableSamples_01[3] = 101.0;
    writableSamples_01[4] = 101.0;

    // convert firs writable sound to first sound
    Sound sound_01 = writableSound_01.toSound();
    QCOMPARE(writableSound_01.isNull(), true);
    QCOMPARE(sound_01.isNull(), false);
    const float *samples_01 = sound_01.samples();
    QCOMPARE(writableSamples_01, samples_01);
    QVERIFY(qFuzzyCompare(samples_01[0], 101.0f));
    QVERIFY(qFuzzyCompare(samples_01[1], 101.0f));
    QVERIFY(qFuzzyCompare(samples_01[2], 101.0f));
    QVERIFY(qFuzzyCompare(samples_01[3], 101.0f));
    QVERIFY(qFuzzyCompare(samples_01[4], 101.0f));

    // convert first sound to second writable sound
    WritableSound writableSound_02;
    QCOMPARE(writableSound_02.fromSound(sound_01), true);
    QCOMPARE(writableSound_02.channelCount(), sound_01.channelCount());
    QCOMPARE(writableSound_02.sampleCount(), sound_01.sampleCount());
    const float *writableSamples_02 = writableSound_02.samples();
    QVERIFY(writableSamples_02 != samples_01);
    QVERIFY(qFuzzyCompare(writableSamples_02[0], samples_01[0]));
    QVERIFY(qFuzzyCompare(writableSamples_02[1], samples_01[1]));
    QVERIFY(qFuzzyCompare(writableSamples_02[2], samples_01[2]));
    QVERIFY(qFuzzyCompare(writableSamples_02[3], 101.0f));
    QVERIFY(qFuzzyCompare(writableSamples_02[4], 101.0f));
}
