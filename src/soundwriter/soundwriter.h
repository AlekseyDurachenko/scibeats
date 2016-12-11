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
#ifndef SOUNDWRITER_H
#define SOUNDWRITER_H


#include "soundwriterworker.h"
#include "soundwriterreply.h"
#include <QObject>


class SoundWriter : public QObject
{
    Q_OBJECT
public:
    SoundWriter(QObject *parent = 0);
    virtual ~SoundWriter() final;

    int activeCount() const;
    int queuedCount() const;
    int totalCount() const;

    SoundWriterReply *write(const SoundWriterRequest &request);

signals:
    void changed();

private slots:
    void reply_aboutToBeAborted();
    void reply_destroyed(QObject *obj);
    void worker_progress(double value, double total);
    void worker_finished(SoundWriterResultState state, const QString &message);
    void writeNext();

private:
    QThread *m_workerWriteThread;
    SoundWriterWorker *m_writerWorker;

    SoundWriterReply *m_activeReply;
    QList<SoundWriterReply *> m_replies;
};


#endif // SOUNDWRITER_H
