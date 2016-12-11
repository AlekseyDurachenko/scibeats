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
#include "test_soundwriterreply.h"
#include "soundwriterreply.h"


Test_SoundWriterReply::Test_SoundWriterReply(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundWriterReply::test_setGet()
{
    SoundWriterReply reply;
    QCOMPARE(reply.request(), SoundWriterRequest());
    QCOMPARE(reply.resultState(), SoundWriterResultState::Success);
    QCOMPARE(reply.resultMessage(), QString());
    QCOMPARE(reply.runningState(), SoundWriterReply::Queue);

    SoundWriterRequest request;
    request.setFileName("/home/abc/abc.wav");

    reply.setRequest(request);
    reply.setResultState(SoundWriterResultState::Aborted);
    reply.setResultMessage(QString("abort msg"));
    reply.setRunningState(SoundWriterReply::Finished);
    QCOMPARE(reply.request(), request);
    QCOMPARE(reply.resultState(), SoundWriterResultState::Aborted);
    QCOMPARE(reply.resultMessage(), QString("abort msg"));
    QCOMPARE(reply.runningState(), SoundWriterReply::Finished);
}
