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
#include "test_soundpeaksreply.h"
#include "soundpeaksreply.h"
#include "writablesound.h"
#include "writablesoundpeaks.h"


Test_SoundPeaksReply::Test_SoundPeaksReply(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundPeaksReply::test_setGet()
{
    SoundPeaksReply reply;
    QCOMPARE(reply.request(), SoundPeaksRequest());
    QCOMPARE(reply.resultState(), SoundPeaksResultState::Success);
    QCOMPARE(reply.resultMessage(), QString());
    QCOMPARE(reply.runningState(), SoundPeaksReply::Queue);
    QCOMPARE(reply.soundPeaks().isNull(), true);

    WritableSound wsnd;
    QCOMPARE(wsnd.create(1, 100), true);
    Sound snd = wsnd.toSound();

    WritableSoundPeaks wsndpeaks;
    QCOMPARE(wsndpeaks.create(snd), true);
    SoundPeaks sndpeaks = wsndpeaks.toSoundPeaks();

    SoundPeaksRequest request;
    request.setSound(snd);

    reply.setRequest(request);
    reply.setResultState(SoundPeaksResultState::Aborted);
    reply.setResultMessage(QString("abort msg"));
    reply.setRunningState(SoundPeaksReply::Finished);
    reply.setSoundPeaks(sndpeaks);
    QCOMPARE(reply.request(), request);
    QCOMPARE(reply.resultState(), SoundPeaksResultState::Aborted);
    QCOMPARE(reply.resultMessage(), QString("abort msg"));
    QCOMPARE(reply.runningState(), SoundPeaksReply::Finished);
    QCOMPARE(reply.soundPeaks(), sndpeaks);
}
