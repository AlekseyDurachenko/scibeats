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
#ifndef SOUNDOBJECT_H
#define SOUNDOBJECT_H


#include "sound.h"
#include "soundtags.h"
#include "soundpeaks.h"
#include <QObject>


class SoundObject : public QObject
{
    Q_OBJECT
public:
    explicit SoundObject(QObject *parent = 0);

    inline bool isNull() const;

    inline const Sound &sound() const;
    inline const SoundTags &tags() const;
    inline const SoundPeaks &peaks() const;

signals:
    void soundChanged();
    void tagsChanged();

    void aboutToBePeaksRecalculated();
    void peaksRecalculated();

public slots:
    void setSound(const Sound &sound);
    void setTags(const SoundTags &tags);

private:
    Sound m_sound;
    SoundTags m_tags;
    SoundPeaks m_peaks;
};

bool SoundObject::isNull() const
{
    return m_sound.isNull();
}

const Sound &SoundObject::sound() const
{
    return m_sound;
}

const SoundTags &SoundObject::tags() const
{
    return m_tags;
}

const SoundPeaks &SoundObject::peaks() const
{
    return m_peaks;
}


#endif // SOUNDOBJECT_H
