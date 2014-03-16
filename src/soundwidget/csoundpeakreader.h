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
#ifndef CSOUNDPEAKREADER_H
#define CSOUNDPEAKREADER_H

#include <QAtomicInt>
#include "qsesinglesignalabstractpeakreader.h"
#include "csound.h"

class CSoundPeakReader : public QseSingleSignalAbstractPeakReader
{
    Q_OBJECT
public:
    explicit CSoundPeakReader(QObject *parent = 0);

    // this method will be called in gui thread
    void abort();
    int start(const QseGeometry &geometry, int width);
    void setSound(const CSound &sound);
    inline const CSound &sound() const;

private slots:
     // this slot work in the separate thread
    void private_start(int id, const CSound &sound,
            const QseGeometry &geometry, int width);

private:
    volatile bool m_isAbort;
    QAtomicInt m_lastId;
    CSound m_lastSound;

};

const CSound &CSoundPeakReader::sound() const
{
    return m_lastSound;
}

#endif // CSOUNDPEAKREADER_H
