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
#include "test_soundproviderreply.h"
#include "soundproviderreply.h"
#include "writablesound.h"


Test_SoundProviderReply::Test_SoundProviderReply(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundProviderReply::test_setGet()
{
    SoundProviderReply reply;
    QCOMPARE(reply.request(), SoundProviderRequest());
    QCOMPARE(reply.resultState(), SoundProviderResultState::Success);
    QCOMPARE(reply.resultMessage(), QString());
    QCOMPARE(reply.runningState(), SoundProviderReply::Queue);
    QCOMPARE(reply.sound().isNull(), true);
    QCOMPARE(reply.tags(), SoundTags());

    SoundProviderRequest request;
    request.setQuery(QByteArray("abcdefg"));

    WritableSound wsnd;
    QVERIFY(wsnd.create(1, 100));
    Sound sound = wsnd.toSound();

    SoundTags tags;
    tags.setArtist("Unknow Artist");

    reply.setRequest(request);
    reply.setResultState(SoundProviderResultState::Aborted);
    reply.setResultMessage(QString("abort msg"));
    reply.setRunningState(SoundProviderReply::Finished);
    reply.setSound(sound);
    reply.setTags(tags);
    QCOMPARE(reply.request(), request);
    QCOMPARE(reply.resultState(), SoundProviderResultState::Aborted);
    QCOMPARE(reply.resultMessage(), QString("abort msg"));
    QCOMPARE(reply.runningState(), SoundProviderReply::Finished);
    QCOMPARE(reply.sound(), sound);
    QCOMPARE(reply.tags(), tags);
}
