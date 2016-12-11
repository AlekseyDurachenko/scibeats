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
#include "test_soundwriter.h"
#include "soundwriter.h"
#include "writablesound.h"
#include "soundtags.h"
#include "soundwriterworker.h"


Test_SoundWriter::Test_SoundWriter(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundWriter::test_write()
{
    WritableSound sound;
    QCOMPARE(sound.create(2, 5), true);
    float *samples = sound.samples();
    samples[0] = -0.9;
    samples[1] = -0.5;
    samples[2] = 0.0;
    samples[3] = 0.5;
    samples[4] = 0.9;
    samples[5] = 0.5;
    samples[6] = 0.0;
    samples[7] = -0.5;
    samples[8] = -0.9;
    samples[9] = -0.5;


    SoundTags soundTags;
    soundTags.setTitle("1");
    soundTags.setCopyright("2");
    soundTags.setSoftware("3");
    soundTags.setArtist("4");
    soundTags.setComment("5");
    soundTags.setDate("6");
    soundTags.setGenre("10");

    const QString &fileName = QDir::tempPath() + QDir::separator() + "test.wav";

    SoundWriterRequest request;
    request.setFileName(fileName);
    request.setFileFormat(SoundFileFormat::WavInt16);
    request.setFileSampleRate(44100);
    request.setSoundSampleRate(44100);
    request.setSound(sound.toSound());
    request.setTags(soundTags);

    //SoundWriterWorker worker;
    //worker.write(request);
    //QCOMPARE(SoundWriter::write(fileName, request), true);
    //qDebug() << "file was saved to " << fileName;
}
