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
#include "test_writablesoundpeaks.h"
#include "writablesoundpeaks.h"
#include "writablesound.h"


Test_WritableSoundPeaks::Test_WritableSoundPeaks(QObject *parent)
    : QObject(parent)
{
}

void Test_WritableSoundPeaks::test_defaultConstructor()
{
    WritableSoundPeaks soundPeaks;
    QCOMPARE(soundPeaks.isNull(), true);
}


void Test_WritableSoundPeaks::test_createAndClear()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 10), true);

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.isNull(), true);
    QCOMPARE(writableSoundPeaks.create(sound), true);
    QCOMPARE(writableSoundPeaks.isNull(), false);

    writableSoundPeaks.clear();
    QCOMPARE(writableSoundPeaks.isNull(), true);

}

void Test_WritableSoundPeaks::test_create_001()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 100), true);
    for (int i = 0; i < 100; ++i) {
        *writableSound.samples(0, i) = i;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QCOMPARE(soundPeaks.count(2), static_cast<qint64>(50));
    QCOMPARE(soundPeaks.count(4), static_cast<qint64>(25));
    QCOMPARE(soundPeaks.count(8), static_cast<qint64>(13));
    QCOMPARE(soundPeaks.count(16), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(32), static_cast<qint64>(4));
    QCOMPARE(soundPeaks.count(64), static_cast<qint64>(2));
    QCOMPARE(soundPeaks.count(128), static_cast<qint64>(1));

    QCOMPARE(soundPeaks.count(2, 2), static_cast<qint64>(49));
    QCOMPARE(soundPeaks.count(16, 2), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 15), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 16), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 17), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 100), static_cast<qint64>(1));
    QCOMPARE(soundPeaks.count(16, 112), static_cast<qint64>(0));
    QCOMPARE(soundPeaks.count(16, 113), static_cast<qint64>(0));

    const float *base = soundPeaks.minimums(0, 2, 0);
    QCOMPARE(soundPeaks.minimums(0, 2), reinterpret_cast<const float *>(base));
    QCOMPARE(soundPeaks.minimums(0, 4), reinterpret_cast<const float *>(base + 50 * 2));
    QCOMPARE(soundPeaks.minimums(0, 8), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2));
    QCOMPARE(soundPeaks.minimums(0, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2));
    QCOMPARE(soundPeaks.minimums(0, 32), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2));
    QCOMPARE(soundPeaks.minimums(0, 64), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2));
    QCOMPARE(soundPeaks.minimums(0, 128), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2));

    QCOMPARE(soundPeaks.maximums(0, 2), reinterpret_cast<const float *>(base + 50));
    QCOMPARE(soundPeaks.maximums(0, 4), reinterpret_cast<const float *>(base + 50 * 2 + 25));
    QCOMPARE(soundPeaks.maximums(0, 8), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13));
    QCOMPARE(soundPeaks.maximums(0, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7));
    QCOMPARE(soundPeaks.maximums(0, 32), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4));
    QCOMPARE(soundPeaks.maximums(0, 64), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2));
    QCOMPARE(soundPeaks.maximums(0, 128), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2 + 1));

    QCOMPARE(soundPeaks.minimums(0, 16, 3), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 1));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 1));

    QCOMPARE(soundPeaks.maximums(0, 16, 3), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7));
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 + 1));
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 + 1));
}

void Test_WritableSoundPeaks::test_create_002()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(2, 100), true);
    for (int i = 0; i < 200; ++i) {
        *writableSound.samples(0, i) = i;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QCOMPARE(soundPeaks.count(2), static_cast<qint64>(50));
    QCOMPARE(soundPeaks.count(4), static_cast<qint64>(25));
    QCOMPARE(soundPeaks.count(8), static_cast<qint64>(13));
    QCOMPARE(soundPeaks.count(16), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(32), static_cast<qint64>(4));
    QCOMPARE(soundPeaks.count(64), static_cast<qint64>(2));
    QCOMPARE(soundPeaks.count(128), static_cast<qint64>(1));

    QCOMPARE(soundPeaks.count(2, 2), static_cast<qint64>(49));
    QCOMPARE(soundPeaks.count(16, 2), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 15), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 16), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 17), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 100), static_cast<qint64>(1));
    QCOMPARE(soundPeaks.count(16, 112), static_cast<qint64>(0));
    QCOMPARE(soundPeaks.count(16, 113), static_cast<qint64>(0));

    const float *base = soundPeaks.minimums(0, 2, 0);
    QCOMPARE(soundPeaks.minimums(0, 2), reinterpret_cast<const float *>(base));
    QCOMPARE(soundPeaks.minimums(0, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 32), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 64), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 128), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2)));

    QCOMPARE(soundPeaks.maximums(0, 2), reinterpret_cast<const float *>(base + 50));
    QCOMPARE(soundPeaks.maximums(0, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2) + 25));
    QCOMPARE(soundPeaks.maximums(0, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2) + 13));
    QCOMPARE(soundPeaks.maximums(0, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 7));
    QCOMPARE(soundPeaks.maximums(0, 32), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2) + 4));
    QCOMPARE(soundPeaks.maximums(0, 64), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2) + 2));
    QCOMPARE(soundPeaks.maximums(0, 128), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2) + 1));

    QCOMPARE(soundPeaks.minimums(0, 16, 3), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 1));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 1));

    QCOMPARE(soundPeaks.maximums(0, 16, 3), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2)) + 7);
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 7 + 1));
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 7 + 1));

    QCOMPARE(soundPeaks.minimums(1, 2), reinterpret_cast<const float *>(base) + (50 * 2));
    QCOMPARE(soundPeaks.minimums(1, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2) + (25 * 2)));
    QCOMPARE(soundPeaks.minimums(1, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2) + (13 * 2)));

    QCOMPARE(soundPeaks.maximums(1, 2), reinterpret_cast<const float *>(base + (50 * 2) + 50));
    QCOMPARE(soundPeaks.maximums(1, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2) + (25 * 2) + 25));
    QCOMPARE(soundPeaks.maximums(1, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2) + (13 * 2) + 13));
}

void Test_WritableSoundPeaks::test_create_003()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 99), true);
    for (int i = 0; i < 99; ++i) {
        *writableSound.samples(0, i) = i;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QCOMPARE(soundPeaks.count(2), static_cast<qint64>(50));
    QCOMPARE(soundPeaks.count(4), static_cast<qint64>(25));
    QCOMPARE(soundPeaks.count(8), static_cast<qint64>(13));
    QCOMPARE(soundPeaks.count(16), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(32), static_cast<qint64>(4));
    QCOMPARE(soundPeaks.count(64), static_cast<qint64>(2));
    QCOMPARE(soundPeaks.count(128), static_cast<qint64>(1));

    QCOMPARE(soundPeaks.count(2, 2), static_cast<qint64>(49));
    QCOMPARE(soundPeaks.count(16, 2), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 15), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 16), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 17), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 100), static_cast<qint64>(1));
    QCOMPARE(soundPeaks.count(16, 112), static_cast<qint64>(0));
    QCOMPARE(soundPeaks.count(16, 113), static_cast<qint64>(0));

    const float *base = soundPeaks.minimums(0, 2, 0);
    QCOMPARE(soundPeaks.minimums(0, 2), reinterpret_cast<const float *>(base));
    QCOMPARE(soundPeaks.minimums(0, 4), reinterpret_cast<const float *>(base + 50 * 2));
    QCOMPARE(soundPeaks.minimums(0, 8), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2));
    QCOMPARE(soundPeaks.minimums(0, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2));
    QCOMPARE(soundPeaks.minimums(0, 32), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2));
    QCOMPARE(soundPeaks.minimums(0, 64), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2));
    QCOMPARE(soundPeaks.minimums(0, 128), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2));

    QCOMPARE(soundPeaks.maximums(0, 2), reinterpret_cast<const float *>(base + 50));
    QCOMPARE(soundPeaks.maximums(0, 4), reinterpret_cast<const float *>(base + 50 * 2 + 25));
    QCOMPARE(soundPeaks.maximums(0, 8), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13));
    QCOMPARE(soundPeaks.maximums(0, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7));
    QCOMPARE(soundPeaks.maximums(0, 32), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4));
    QCOMPARE(soundPeaks.maximums(0, 64), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2));
    QCOMPARE(soundPeaks.maximums(0, 128), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2 + 1));

    QCOMPARE(soundPeaks.minimums(0, 16, 3), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 1));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 1));

    QCOMPARE(soundPeaks.maximums(0, 16, 3), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7));
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 + 1));
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 50 * 2 + 25 * 2 + 13 * 2 + 7 + 1));
}

void Test_WritableSoundPeaks::test_create_004()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(2, 99), true);
    for (int i = 0; i < 198; ++i) {
        *writableSound.samples(0, i) = i;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QCOMPARE(soundPeaks.count(2), static_cast<qint64>(50));
    QCOMPARE(soundPeaks.count(4), static_cast<qint64>(25));
    QCOMPARE(soundPeaks.count(8), static_cast<qint64>(13));
    QCOMPARE(soundPeaks.count(16), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(32), static_cast<qint64>(4));
    QCOMPARE(soundPeaks.count(64), static_cast<qint64>(2));
    QCOMPARE(soundPeaks.count(128), static_cast<qint64>(1));

    QCOMPARE(soundPeaks.count(2, 2), static_cast<qint64>(49));
    QCOMPARE(soundPeaks.count(16, 2), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 15), static_cast<qint64>(7));
    QCOMPARE(soundPeaks.count(16, 16), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 17), static_cast<qint64>(6));
    QCOMPARE(soundPeaks.count(16, 100), static_cast<qint64>(1));
    QCOMPARE(soundPeaks.count(16, 112), static_cast<qint64>(0));
    QCOMPARE(soundPeaks.count(16, 113), static_cast<qint64>(0));

    const float *base = soundPeaks.minimums(0, 2, 0);
    QCOMPARE(soundPeaks.minimums(0, 2), reinterpret_cast<const float *>(base));
    QCOMPARE(soundPeaks.minimums(0, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 32), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 64), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 128), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2)));

    QCOMPARE(soundPeaks.maximums(0, 2), reinterpret_cast<const float *>(base + 50));
    QCOMPARE(soundPeaks.maximums(0, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2) + 25));
    QCOMPARE(soundPeaks.maximums(0, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2) + 13));
    QCOMPARE(soundPeaks.maximums(0, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 7));
    QCOMPARE(soundPeaks.maximums(0, 32), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2) + 4));
    QCOMPARE(soundPeaks.maximums(0, 64), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2) + 2));
    QCOMPARE(soundPeaks.maximums(0, 128), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2 + 7 * 2 + 4 * 2 + 2 * 2) + 1));

    QCOMPARE(soundPeaks.minimums(0, 16, 3), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2)));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 1));
    QCOMPARE(soundPeaks.minimums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 1));

    QCOMPARE(soundPeaks.maximums(0, 16, 3), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2)) + 7);
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 7 + 1));
    QCOMPARE(soundPeaks.maximums(0, 16, 16), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2 + 13 * 2) + 7 + 1));

    QCOMPARE(soundPeaks.minimums(1, 2), reinterpret_cast<const float *>(base) + (50 * 2));
    QCOMPARE(soundPeaks.minimums(1, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2) + (25 * 2)));
    QCOMPARE(soundPeaks.minimums(1, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2) + (13 * 2)));

    QCOMPARE(soundPeaks.maximums(1, 2), reinterpret_cast<const float *>(base + (50 * 2) + 50));
    QCOMPARE(soundPeaks.maximums(1, 4), reinterpret_cast<const float *>(base + 2 * (50 * 2) + (25 * 2) + 25));
    QCOMPARE(soundPeaks.maximums(1, 8), reinterpret_cast<const float *>(base + 2 * (50 * 2 + 25 * 2) + (13 * 2) + 13));
}

void Test_WritableSoundPeaks::test_samples_001()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 100), true);
    for (int i = 0; i < 100; ++i) {
        *writableSound.samples(0, i) = i - 50.0f;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 0), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 1), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 98), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 99), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 99), 49.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 0), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 1), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 2), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 2), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 3), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 3), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 96), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 96), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 97), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 97), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 98), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 98), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 99), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 99), 49.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 0), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 7), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 7), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 15), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 15), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 16), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 16), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 21), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 21), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 31), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 31), -19.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 128, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 128, 0), +49.0f));
}

void Test_WritableSoundPeaks::test_samples_002()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(2, 100), true);
    for (int i = 0; i < 100; ++i) {
        *writableSound.samples(0, i) = i - 50.0f;
        *writableSound.samples(1, i) = i - 50.0f;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 0), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 1), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 98), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 99), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 99), 49.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 0), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 1), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 2), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 2), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 3), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 3), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 96), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 96), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 97), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 97), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 98), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 98), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 99), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 99), 49.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 0), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 7), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 7), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 15), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 15), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 16), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 16), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 21), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 21), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 31), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 31), -19.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 128, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 128, 0), +49.0f));


    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 0), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 1), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 98), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 99), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 99), 49.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 0), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 1), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 2), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 2), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 3), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 3), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 96), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 96), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 97), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 97), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 98), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 98), 49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 99), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 99), 49.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 0), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 7), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 7), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 15), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 15), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 16), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 16), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 21), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 21), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 31), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 31), -19.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 128, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 128, 0), +49.0f));
}

void Test_WritableSoundPeaks::test_samples_003()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 99), true);
    for (int i = 0; i < 99; ++i) {
        *writableSound.samples(0, i) = i - 50.0f;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 0), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 1), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 99), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 99), 48.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 0), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 1), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 2), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 2), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 3), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 3), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 96), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 96), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 97), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 97), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 98), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 99), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 99), 48.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 0), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 7), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 7), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 15), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 15), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 16), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 16), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 21), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 21), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 31), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 31), -19.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 128, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 128, 0), +48.0f));
}

void Test_WritableSoundPeaks::test_samples_004()
{
    WritableSound writableSound;
    QCOMPARE(writableSound.create(2, 99), true);
    for (int i = 0; i < 99; ++i) {
        *writableSound.samples(0, i) = i - 50.0f;
        *writableSound.samples(1, i) = i - 50.0f;
    }

    Sound sound = writableSound.toSound();
    QCOMPARE(sound.isNull(), false);

    WritableSoundPeaks writableSoundPeaks;
    QCOMPARE(writableSoundPeaks.create(sound), true);

    SoundPeaks soundPeaks = writableSoundPeaks.toSoundPeaks();
    QCOMPARE(soundPeaks.isNull(), false);
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 0), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 1), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 2, 99), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 2, 99), 48.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 0), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 1), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 2), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 2), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 3), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 3), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 96), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 96), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 97), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 97), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 98), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 4, 99), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 4, 99), 48.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 0), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 7), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 7), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 15), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 15), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 16), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 16), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 21), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 21), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 16, 31), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 16, 31), -19.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(0, 128, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(0, 128, 0), +48.0f));


    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 0), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 1), -49.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 2, 99), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 2, 99), 48.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 0), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 1), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 1), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 2), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 2), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 3), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 3), -47.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 96), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 96), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 97), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 97), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 98), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 98), 48.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 4, 99), 46.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 4, 99), 48.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 0), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 7), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 7), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 15), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 15), -35.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 16), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 16), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 21), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 21), -19.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 16, 31), -34.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 16, 31), -19.0f));

    QVERIFY(qFuzzyCompare(*soundPeaks.minimums(1, 128, 0), -50.0f));
    QVERIFY(qFuzzyCompare(*soundPeaks.maximums(1, 128, 0), +48.0f));
}
