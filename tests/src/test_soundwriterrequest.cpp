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
#include "test_soundwriterrequest.h"
#include "soundwriterrequest.h"
#include "writablesound.h"


Test_SoundWriterRequest::Test_SoundWriterRequest(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundWriterRequest::test_setGet()
{
    SoundWriterRequest request;
    QCOMPARE(request.isNull(), true);

    request.setFileName("/tmp/123.wav");
    QCOMPARE(request.fileName(), QString("/tmp/123.wav"));
    QCOMPARE(request.isNull(), true);

    WritableSound writableSound;
    QCOMPARE(writableSound.create(1, 10), true);
    Sound sound = writableSound.toSound();
    SoundTags tags;
    request.setSound(sound);
    request.setTags(tags);
    request.setFileFormat(SoundFileFormat::WavInt24);
    QCOMPARE(request.isNull(), false);
    QCOMPARE(request.sound(), sound);
    QCOMPARE(request.tags(), tags);
    QCOMPARE(request.fileFormat(), SoundFileFormat::WavInt24);

    // test samplerate methods
    QVERIFY(qFuzzyCompare(request.fileSampleRate(), 44100.0));
    QVERIFY(qFuzzyCompare(request.soundSampleRate(), 44100.0));
    request.setFileSampleRate(22050.0);
    request.setSoundSampleRate(22060.0);
    QVERIFY(qFuzzyCompare(request.fileSampleRate(), 22050.0));
    QVERIFY(qFuzzyCompare(request.soundSampleRate(), 22060.0));
}

void Test_SoundWriterRequest::test_eq()
{
    SoundWriterRequest request1;
    request1.setFileName("/tmp/123.wav");
    WritableSound writableSound1;
    QCOMPARE(writableSound1.create(1, 10), true);
    Sound sound1 = writableSound1.toSound();
    SoundTags tags1;
    request1.setSound(sound1);
    request1.setTags(tags1);
    request1.setFileFormat(SoundFileFormat::WavInt24);
    request1.setFileSampleRate(22050.0);
    request1.setSoundSampleRate(22060.0);


    SoundWriterRequest request2;
    request2.setFileName("/tmp/123.wav");
    WritableSound writableSound2;
    QCOMPARE(writableSound2.create(1, 10), true);
    Sound sound2 = writableSound2.toSound();
    SoundTags tags2;
    request2.setSound(sound2);
    request2.setTags(tags2);
    request2.setFileFormat(SoundFileFormat::WavInt24);
    request2.setFileSampleRate(22050.0);
    request2.setSoundSampleRate(22070.0);


    SoundWriterRequest request3;
    request3.setFileName("/tmp/123.wav");
    SoundTags tags3;
    request3.setSound(sound1);
    request3.setTags(tags3);
    request3.setFileFormat(SoundFileFormat::WavInt24);
    request3.setFileSampleRate(22050.0);
    request3.setSoundSampleRate(22060.0);

    QVERIFY(request1 == request1);
    QVERIFY(request1 != request2);
    QVERIFY(request1 == request3);
}
