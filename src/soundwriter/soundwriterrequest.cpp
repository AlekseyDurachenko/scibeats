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
#include "soundwriterrequest.h"


SoundWriterRequest::SoundWriterRequest()
{
    m_fileFormat = SoundFileFormat::WavInt16;
    m_fileSampleRate = 44100.0;
    m_soundSampleRate = 44100.0;
}

void SoundWriterRequest::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void SoundWriterRequest::setSound(const Sound &sound)
{
    m_sound = sound;
}

void SoundWriterRequest::setTags(const SoundTags &tags)
{
    m_tags = tags;
}

void SoundWriterRequest::setFileFormat(SoundFileFormat fileFormat)
{
    m_fileFormat = fileFormat;
}

void SoundWriterRequest::setFileSampleRate(double sampleRate)
{
    m_fileSampleRate = sampleRate;
}

void SoundWriterRequest::setSoundSampleRate(double sampleRate)
{
    m_soundSampleRate = sampleRate;
}
