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
#include "test_soundpeaksrequest.h"
#include "soundpeaksrequest.h"
#include "writablesound.h"


Test_SoundPeaksRequest::Test_SoundPeaksRequest(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundPeaksRequest::test_setGet()
{
    SoundPeaksRequest request;
    QCOMPARE(request.isNull(), true);

    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 10), true);
    Sound sound = writableSound.toSound();
    request.setSound(sound);
    QCOMPARE(request.isNull(), false);
    QCOMPARE(request.sound(), sound);
}

void Test_SoundPeaksRequest::test_eq()
{
    WritableSound wsnd1;
    WritableSound wsnd2;
    QVERIFY(wsnd1.create(1, 100));
    QVERIFY(wsnd2.create(2, 100));
    Sound snd1 = wsnd1.toSound();
    Sound snd2 = wsnd2.toSound();
    SoundPeaksRequest req1;
    SoundPeaksRequest req2;
    SoundPeaksRequest req3;
    req1.setSound(snd1);
    req2.setSound(snd2);
    req3.setSound(snd1);
    QVERIFY(req1 == req1);
    QVERIFY(req1 != req2);
    QVERIFY(req1 == req3);
}
