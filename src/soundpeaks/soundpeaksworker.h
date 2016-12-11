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
#ifndef SOUNDPEAKSWORKER_H
#define SOUNDPEAKSWORKER_H


#include "soundpeaksresultstate.h"
#include "soundpeaksrequest.h"
#include <QObject>


class SoundPeaksWorker : public QObject
{
    Q_OBJECT
public:
    explicit SoundPeaksWorker(QObject *parent = 0);

    bool isFinished() const;
    bool isRunning() const;
    bool isAborted() const;

    void write(const SoundPeaksRequest &request);
    void abort();

signals:
    void progress(double value, double total);
    void finished(SoundPeaksResultState state, const SoundPeaks &peaks, const QString &message);

private slots:
    void privateWrite(const SoundPeaksRequest &request);

private:
    QAtomicInt m_running;
    QAtomicInt m_aborted;
};


#endif // SOUNDPEAKSWORKER_H
