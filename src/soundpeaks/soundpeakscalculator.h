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
#ifndef SOUNDPEAKSCALCULATOR_H
#define SOUNDPEAKSCALCULATOR_H


#include "soundpeaksworker.h"
#include "soundpeaksreply.h"
#include <QObject>


class SoundPeaksCalculator : public QObject
{
    Q_OBJECT
public:
    SoundPeaksCalculator(QObject *parent = 0);
    virtual ~SoundPeaksCalculator() final;

    int activeCount() const;
    int queuedCount() const;
    int totalCount() const;

    SoundPeaksReply *calc(const SoundPeaksRequest &request);

signals:
    void changed();

private slots:
    void reply_aboutToBeAborted();
    void reply_destroyed(QObject *obj);
    void worker_progress(double value, double total);
    void worker_finished(SoundPeaksResultState state,
                         const SoundPeaks &soundPeaks,
                         const QString &message);
    void writeNext();

private:
    QThread *m_workerThread;
    SoundPeaksWorker *m_worker;

    SoundPeaksReply *m_activeReply;
    QList<SoundPeaksReply *> m_replies;
};


#endif // SOUNDPEAKSCALCULATOR_H
