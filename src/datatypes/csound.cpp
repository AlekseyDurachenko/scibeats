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
#include "csound.h"
#include <QDebug>

CSound::CSound()
{
    d = new CSoundData;

    d->m_mapFile.setAutoRemove(true);
    d->m_sampleRate = 44100;
    d->m_frameCount = 0;
    d->m_frames = 0;
}

CSound::CSound(const CSound &other) : d(other.d)
{
}

void CSound::setProperty(CSound::PropertyType type, QString value)
{
    QMutexLocker locker(&d->m_mutex);

    d->m_properties[type] = value;
}

QString CSound::property(CSound::PropertyType type) const
{
    QMutexLocker locker(&d->m_mutex);

    if (d->m_properties.contains(type))
    {
        return d->m_properties.value(type);
    }

    return QString();
}

void CSound::setSampleRate(double sampleRate)
{
    d->m_sampleRate = sampleRate;
}

bool CSound::resize(qint64 frameCount)
{
    if (!d->m_mapFile.isOpen())
    {
        if (!d->m_mapFile.open())
        {
            qWarning() << "Can't create temporary file"
                    << d->m_mapFile.fileName();
        }
    }

    if (d->m_mapFile.resize(frameCount * sizeof(float)))
    {
        d->m_frames = reinterpret_cast<float *>(
                d->m_mapFile.map(0, frameCount * sizeof(float)));
        d->m_frameCount = frameCount;
        return true;
    }

    return false;
}

void CSound::write(qint64 offset, float *data, qint64 frameCount)
{
    memcpy(&d->m_frames[offset], data, frameCount * sizeof(float));
}

void CSound::read(qint64 offset, float *data, qint64 frameCount) const
{
    memcpy(data, &d->m_frames[offset], frameCount * sizeof(float));
}
