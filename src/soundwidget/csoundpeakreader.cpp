// Copyright 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "csoundpeakreader.h"

CSoundPeakReader::CSoundPeakReader(QObject *parent) :
    QseSingleSignalAbstractPeakReader(parent)
{
    m_isAbort = false;
    m_lastId = 0;
}

void CSoundPeakReader::abort()
{
    m_isAbort = true;
}

int CSoundPeakReader::start(const QseGeometry &geometry, int width)
{
    int last_id = m_lastId.fetchAndAddOrdered(1) + 1;
    m_isAbort = true;
    QMetaObject::invokeMethod(this, "private_start", Qt::QueuedConnection,
            Q_ARG(int, last_id), Q_ARG(CSound, m_lastSound), Q_ARG(QseGeometry, geometry), Q_ARG(int, width));
    return last_id;
}

void CSoundPeakReader::setSound(const CSound &sound)
{
    m_lastSound = sound;
    m_isAbort = true;
}

void CSoundPeakReader::private_start(int id, const CSound &sound, const QseGeometry &geometry, int width)
{
    m_isAbort = false;
    if (id == m_lastId && sound == m_lastSound)
    {
        QList<float> minimums, maximums;
        qint64 samplePerPixel = geometry.samplePerPixel();
        qint64 sampleCount = sound.frameCount();
        float *samples = sound.frames();

        if (samplePerPixel > 0)
        {
            qint64 offset = geometry.x() * samplePerPixel;

            for (int pixel = 0; pixel < width && !m_isAbort; ++pixel)
            {
                int pixelFirstIndex = offset + pixel * samplePerPixel;
                if (pixelFirstIndex >= sampleCount)
                {
                    break;
                }

                float min = samples[pixelFirstIndex];
                float max = min;
                for (qint64 sample = 0; sample < samplePerPixel && !m_isAbort; ++sample)
                {
                    qint64 index = pixelFirstIndex + sample;
                    if (index >= sampleCount)
                    {
                        break;
                    }

                    if (samples[index] > max)
                    {
                        max = samples[index];
                    }
                    else if (samples[index] < min)
                    {
                        min = samples[index];
                    }
                }

                if (!m_isAbort)
                {
                    minimums.push_back(min);
                    maximums.push_back(max);
                }
            }
        }
        else
        {
            qint64 offset = geometry.x();
            qint64 count = width / qAbs(geometry.samplePerPixel()) + 1
                    + ((width%qAbs(geometry.samplePerPixel())) ? 1 : 0);

            for (int index = offset; index < offset + count && index < sampleCount; ++index)
            {
                minimums.push_back(samples[index]);
            }
        }

        if (!m_isAbort)
        {
            emit peakReaded(id, geometry, minimums, maximums);
        }
    }
}
