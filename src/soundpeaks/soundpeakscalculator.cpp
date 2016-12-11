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
#include "soundpeakscalculator.h"
#include <QThread>
#include <QDebug>


SoundPeaksCalculator::SoundPeaksCalculator(QObject *parent)
    : QObject(parent)
{
    m_workerThread = new QThread(this);
    m_workerThread->start();

    m_worker = new SoundPeaksWorker;
    m_worker->moveToThread(m_workerThread);
    connect(m_worker, SIGNAL(finished(SoundPeaksResultState, SoundPeaks, QString)),
            this, SLOT(worker_finished(SoundPeaksResultState, SoundPeaks, QString)));
    connect(m_worker, SIGNAL(progress(double, double)),
            this, SLOT(worker_progress(double, double)));

    m_activeReply = nullptr;
}

SoundPeaksCalculator::~SoundPeaksCalculator()
{
    m_replies.clear();
    m_activeReply = nullptr;

    m_worker->abort();
    m_workerThread->quit();
    m_workerThread->wait(10000);

    delete m_worker;
    delete m_workerThread;
}

int SoundPeaksCalculator::activeCount() const
{
    if (m_activeReply) {
        return 1;
    }

    return 0;
}

int SoundPeaksCalculator::queuedCount() const
{
    return totalCount() - activeCount();
}

int SoundPeaksCalculator::totalCount() const
{
    return m_replies.count();
}

SoundPeaksReply *SoundPeaksCalculator::calc(const SoundPeaksRequest &request)
{
    SoundPeaksReply *reply = new SoundPeaksReply(this);
    reply->setRequest(request);
    reply->setRunningState(SoundPeaksReply::Queue);
    connect(reply, SIGNAL(aboutToBeAborted()),
            this, SLOT(reply_aboutToBeAborted()));
    connect(reply, SIGNAL(destroyed(QObject *)),
            this, SLOT(reply_destroyed(QObject *)));
    m_replies.append(reply);

    QMetaObject::invokeMethod(this, "writeNext", Qt::QueuedConnection);

    emit changed();

    return reply;
}

void SoundPeaksCalculator::reply_aboutToBeAborted()
{
    SoundPeaksReply *reply = qobject_cast<SoundPeaksReply *>(sender());
    if (m_activeReply == reply) {
        reply->setRunningState(SoundPeaksReply::Aborting);
        m_worker->abort();
    }
    else {
        m_replies.removeOne(reply);
        reply->setResultState(SoundPeaksResultState::Aborted);
        reply->setResultMessage(tr("aborted"));
        reply->setRunningState(SoundPeaksReply::Finished);
        reply->finish();

        emit changed();
    }
}

void SoundPeaksCalculator::reply_destroyed(QObject *reply)
{
    if (m_activeReply == reply) {
        m_worker->abort();
        m_activeReply = nullptr;
    }

    m_replies.removeOne(reinterpret_cast<SoundPeaksReply *>(reply));

    emit changed();
}

void SoundPeaksCalculator::worker_progress(double value, double total)
{
    if (m_activeReply) {
        m_activeReply->setProgress(value, total);
    }
}

void SoundPeaksCalculator::worker_finished(SoundPeaksResultState state,
                                           const SoundPeaks &soundPeaks,
                                           const QString &message)
{
    if (m_activeReply) {
        m_activeReply->setResultState(state);
        m_activeReply->setResultMessage(message);
        m_activeReply->setSoundPeaks(soundPeaks);
        m_activeReply->setRunningState(SoundPeaksReply::Finished);
        m_activeReply->finish();

        m_replies.removeOne(m_activeReply);
        m_activeReply = nullptr;

        emit changed();
    }

    QMetaObject::invokeMethod(this, "writeNext", Qt::QueuedConnection);
}

void SoundPeaksCalculator::writeNext()
{
    if (m_activeReply != nullptr || m_replies.isEmpty()) {
        return;
    }

    m_activeReply = m_replies.first();
    m_activeReply->setRunningState(SoundPeaksReply::Running);
    m_worker->write(m_activeReply->request());

    emit changed();
}
