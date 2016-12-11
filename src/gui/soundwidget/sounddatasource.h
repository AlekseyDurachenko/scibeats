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
#ifndef SOUNDDATASOURCE_H
#define SOUNDDATASOURCE_H


#include "sound.h"
#include "soundpeaks.h"
#include "soundchanneldatasource.h"
#include "qseabstractsignaldatasource.h"
#include <QObject>


class SoundDataSource : public QseAbstractSignalDataSource
{
    Q_OBJECT
public:
    explicit SoundDataSource(QObject *parent = 0);

    inline const Sound &sound() const;
    inline const SoundPeaks &peaks() const;

    int channel() const;
    bool isNull() const;

    virtual qint64 count() const final;
    virtual qint64 minIndex() const final;
    virtual qint64 maxIndex() const final;

    virtual qint64 readAsPoints(double *points,
                                qint64 index,
                                qint64 count)  final;

    virtual qint64 readAsPeaks(double *minimums,
                               double *maximums,
                               qint64 index,
                               qint64 spp,
                               qint64 count) final;

public slots:
    void setSound(const Sound &sound);

private:
    Sound m_sound;
    SoundPeaks m_peaks;
};

const Sound &SoundDataSource::sound() const
{
    return m_sound;
}

const SoundPeaks &SoundDataSource::peaks() const
{
    return m_peaks;
}


#endif // SOUNDDATASOURCE_H
