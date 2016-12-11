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
#ifndef SOUNDWRITERWORKER_H
#define SOUNDWRITERWORKER_H


#include "soundwriterresultstate.h"
#include "soundwriterrequest.h"
#include <QObject>


class SoundWriterWorker : public QObject
{
    Q_OBJECT
public:
    explicit SoundWriterWorker(QObject *parent = 0);

    bool isFinished() const;
    bool isRunning() const;
    bool isAborted() const;

    void write(const SoundWriterRequest &request);
    void abort();

signals:
    void progress(double value, double total);
    void finished(SoundWriterResultState state, const QString &message);

private slots:
    void privateWrite(const SoundWriterRequest &request);
    bool privateWrite(const SoundWriterRequest &request, QString &reason);

private:
    QAtomicInt m_running;
    QAtomicInt m_aborted;
};


#endif // SOUNDWRITERWORKER_H
