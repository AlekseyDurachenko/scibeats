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
#include "soundwriter.h"
#include <QThread>
#include <QDebug>


SoundWriter::SoundWriter(QObject *parent)
    : QObject(parent)
{
    m_workerWriteThread = new QThread(this);
    m_workerWriteThread->start();

    m_writerWorker = new SoundWriterWorker;
    m_writerWorker->moveToThread(m_workerWriteThread);
    connect(m_writerWorker, SIGNAL(finished(SoundWriterResultState, QString)),
            this, SLOT(worker_finished(SoundWriterResultState, QString)));
    connect(m_writerWorker, SIGNAL(progress(double, double)),
            this, SLOT(worker_progress(double, double)));

    m_activeReply = nullptr;
}

SoundWriter::~SoundWriter()
{
    m_replies.clear();
    m_activeReply = nullptr;

    m_writerWorker->abort();
    m_workerWriteThread->quit();
    m_workerWriteThread->wait(10000);

    delete m_writerWorker;
    delete m_workerWriteThread;
}

int SoundWriter::activeCount() const
{
    if (m_activeReply) {
        return 1;
    }

    return 0;
}

int SoundWriter::queuedCount() const
{
    return totalCount() - activeCount();
}

int SoundWriter::totalCount() const
{
    return m_replies.count();
}

SoundWriterReply *SoundWriter::write(const SoundWriterRequest &request)
{
    SoundWriterReply *reply = new SoundWriterReply(this);
    reply->setRequest(request);
    reply->setRunningState(SoundWriterReply::Queue);
    connect(reply, SIGNAL(aboutToBeAborted()),
            this, SLOT(reply_aboutToBeAborted()));
    connect(reply, SIGNAL(destroyed(QObject *)),
            this, SLOT(reply_destroyed(QObject *)));
    m_replies.append(reply);

    QMetaObject::invokeMethod(this, "writeNext", Qt::QueuedConnection);

    emit changed();

    return reply;
}

void SoundWriter::reply_aboutToBeAborted()
{
    SoundWriterReply *reply = qobject_cast<SoundWriterReply *>(sender());
    if (m_activeReply == reply) {
        reply->setRunningState(SoundWriterReply::Aborting);
        m_writerWorker->abort();
    }
    else {
        m_replies.removeOne(reply);
        reply->setResultState(SoundWriterResultState::Aborted);
        reply->setResultMessage(tr("aborted"));
        reply->setRunningState(SoundWriterReply::Finished);
        reply->finish();

        emit changed();
    }
}

void SoundWriter::reply_destroyed(QObject *reply)
{
    if (m_activeReply == reply) {
        m_writerWorker->abort();
        m_activeReply = nullptr;
    }

    m_replies.removeOne(reinterpret_cast<SoundWriterReply *>(reply));

    emit changed();
}

void SoundWriter::worker_progress(double value, double total)
{
    if (m_activeReply) {
        m_activeReply->setProgress(value, total);
    }
}

void SoundWriter::worker_finished(SoundWriterResultState state, const QString &message)
{
    if (m_activeReply) {
        m_activeReply->setResultState(state);
        m_activeReply->setResultMessage(message);
        m_activeReply->setRunningState(SoundWriterReply::Finished);
        m_activeReply->finish();

        m_replies.removeOne(m_activeReply);
        m_activeReply = nullptr;

        emit changed();
    }

    QMetaObject::invokeMethod(this, "writeNext", Qt::QueuedConnection);
}

void SoundWriter::writeNext()
{
    if (m_activeReply != nullptr || m_replies.isEmpty()) {
        return;
    }

    m_activeReply = m_replies.first();
    m_activeReply->setRunningState(SoundWriterReply::Running);
    m_writerWorker->write(m_activeReply->request());

    emit changed();
}
