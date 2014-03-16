// Copyright 2013-2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef CSOUND_H
#define CSOUND_H

#include <QExplicitlySharedDataPointer>
#include "csounddata.h"

// This object is explicity shared. You mustn't change the data after
// the start of use.
class CSound
{
public:
    enum PropertyType {Title, Copyright, Software, Artist, Comment,
            Date, Album, License, TrackNumber, Genre};

    CSound();
    CSound(const CSound &other);

    // the thread-safe methods
    void setProperty(PropertyType type, QString value);
    QString property(CSound::PropertyType type) const;

    // do not use this method after creating the object
    bool resize(qint64 frameCount);
    void setSampleRate(double sampleRate);
    void write(qint64 offset, float *data, qint64 frameCount);
    inline float *frames();

    // can be used everywere
    inline double sampleRate() const;
    void read(qint64 offset, float *data, qint64 frameCount) const;
    inline qint64 frameCount() const;
    inline float *frames() const;
    inline bool isNull() const;

    inline bool operator == (const CSound& other) const;
    inline bool operator != (const CSound& other) const;

private:
    QExplicitlySharedDataPointer<CSoundData> d;

};

float *CSound::frames()
{
    return d->m_frames;
}

double CSound::sampleRate() const
{
    return d->m_sampleRate;
}

qint64 CSound::frameCount() const
{
    return d->m_frameCount;
}

float *CSound::frames() const
{
    return d->m_frames;
}

bool CSound::isNull() const
{
    return d->m_frames == 0;
}

bool CSound::operator == (const CSound& other) const
{
    return d == other.d;
}

bool CSound::operator != (const CSound& other) const
{
    return d != other.d;
}

#endif // CSOUND_H
