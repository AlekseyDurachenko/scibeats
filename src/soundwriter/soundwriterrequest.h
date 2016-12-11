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
#ifndef SOUNDWRITERREQUEST_H
#define SOUNDWRITERREQUEST_H


#include "sound.h"
#include "soundtags.h"
#include "soundfileformat.h"
#include <QMetaType>


class SoundWriterRequest
{
public:
    SoundWriterRequest();

    inline bool isNull() const;

    inline const QString &fileName() const;
    void setFileName(const QString &fileName);

    inline const Sound &sound() const;
    void setSound(const Sound &sound);

    inline const SoundTags &tags() const;
    void setTags(const SoundTags &tags);

    inline SoundFileFormat fileFormat() const;
    void setFileFormat(SoundFileFormat fileFormat);

    inline double fileSampleRate() const;
    void setFileSampleRate(double sampleRate);

    inline double soundSampleRate() const;
    void setSoundSampleRate(double sampleRate);

    inline bool operator ==(const SoundWriterRequest &other) const;
    inline bool operator !=(const SoundWriterRequest &other) const;

private:
    QString m_fileName;
    Sound m_sound;
    SoundTags m_tags;
    SoundFileFormat m_fileFormat;
    double m_fileSampleRate;
    double m_soundSampleRate;
};
Q_DECLARE_METATYPE(SoundWriterRequest)


bool SoundWriterRequest::isNull() const
{
    return m_sound.isNull() || m_fileName.isNull();
}

const QString &SoundWriterRequest::fileName() const
{
    return m_fileName;
}

const Sound &SoundWriterRequest::sound() const
{
    return m_sound;
}

const SoundTags &SoundWriterRequest::tags() const
{
    return m_tags;
}

SoundFileFormat SoundWriterRequest::fileFormat() const
{
    return m_fileFormat;
}

double SoundWriterRequest::fileSampleRate() const
{
    return m_fileSampleRate;
}

double SoundWriterRequest::soundSampleRate() const
{
    return m_soundSampleRate;
}

bool SoundWriterRequest::operator ==(const SoundWriterRequest &other) const
{
    return m_fileName == other.m_fileName
           && m_sound == other.m_sound
           && m_tags == other.m_tags
           && m_fileFormat == other.m_fileFormat
           && qFuzzyCompare(m_fileSampleRate, other.m_fileSampleRate)
           && qFuzzyCompare(m_soundSampleRate, other.m_soundSampleRate);
}

bool SoundWriterRequest::operator !=(const SoundWriterRequest &other) const
{
    return !operator==(other);
}


#endif // SOUNDWRITERREQUEST_H
