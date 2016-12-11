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
#include "soundprovider.h"
#include "plugin_soundprovider_factory.h"
#include <QDebug>


SoundProvider::SoundProvider(QNetworkAccessManager *network, QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(network);

    m_network = network;
    m_worker = nullptr;
    m_activeReply = nullptr;
}

SoundProvider::~SoundProvider()
{
    m_replies.clear();
}

QNetworkAccessManager *SoundProvider::network() const
{
    return m_network;
}

int SoundProvider::activeCount() const
{
    if (m_activeReply) {
        return 1;
    }

    return 0;
}

int SoundProvider::queuedCount() const
{
    return totalCount() - activeCount();
}

int SoundProvider::totalCount() const
{
    return m_replies.count();
}

SoundProviderReply *SoundProvider::get(const SoundProviderRequest &request, QString *reason)
{
    if (!plugin::soundprovider::Factory::validateRequest(request, reason)) {
        return 0;
    }

    SoundProviderReply *reply = new SoundProviderReply(this);
    reply->setRequest(request);
    reply->setRunningState(SoundProviderReply::Queue);
    connect(reply, SIGNAL(aboutToBeAborted()),
            this, SLOT(reply_aboutToBeAborted()));
    connect(reply, SIGNAL(destroyed(QObject *)),
            this, SLOT(reply_destroyed(QObject *)));
    m_replies.append(reply);

    QMetaObject::invokeMethod(this, "next", Qt::QueuedConnection);

    emit changed();

    return reply;
}

void SoundProvider::reply_aboutToBeAborted()
{
    SoundProviderReply *reply = qobject_cast<SoundProviderReply *>(sender());
    if (m_activeReply == reply) {
        reply->setRunningState(SoundProviderReply::Aborting);
        m_worker->abort();
    }
    else {
        m_replies.removeOne(reply);
        reply->setResultState(SoundProviderResultState::Aborted);
        reply->setResultMessage(tr("aborted"));
        reply->setRunningState(SoundProviderReply::Finished);
        reply->finish();

        emit changed();
    }
}

void SoundProvider::reply_destroyed(QObject *reply)
{
    if (m_activeReply == reply) {
        m_worker->abort();
        m_worker->deleteLater();
        m_activeReply = nullptr;
        m_worker = nullptr;
    }

    m_replies.removeOne(reinterpret_cast<SoundProviderReply *>(reply));

    emit changed();
}

void SoundProvider::worker_progress(double value, double total)
{
    if (m_activeReply) {
        m_activeReply->setProgress(value, total);
    }
}

void SoundProvider::worker_progress(const LogMessage &logMessage)
{
    if (m_activeReply) {
        m_activeReply->setProgress(logMessage);
    }
}

void SoundProvider::worker_finished(SoundProviderResultState state,
                                    const Sound &sound,
                                    const SoundTags &tags,
                                    const QString &message)
{
    if (m_activeReply) {
        m_activeReply->setResultState(state);
        m_activeReply->setSound(sound);
        m_activeReply->setTags(tags);
        m_activeReply->setResultMessage(message);
        m_activeReply->setRunningState(SoundProviderReply::Finished);
        m_activeReply->finish();

        m_replies.removeOne(m_activeReply);
        m_activeReply = nullptr;

        m_worker->deleteLater();
        m_worker = nullptr;

        emit changed();
    }

    QMetaObject::invokeMethod(this, "next", Qt::QueuedConnection);
}

void SoundProvider::next()
{
    if (m_activeReply || m_worker || m_replies.isEmpty()) {
        return;
    }

    m_worker = plugin::soundprovider::Factory::createWorker(m_replies.first()->request(), network(), this);
    connect(m_worker, SIGNAL(finished(SoundProviderResultState, Sound, SoundTags, QString)),
            this, SLOT(worker_finished(SoundProviderResultState, Sound, SoundTags, QString)));
    connect(m_worker, SIGNAL(progress(double, double)),
            this, SLOT(worker_progress(double, double)));
    connect(m_worker, SIGNAL(progress(LogMessage)),
            this, SLOT(worker_progress(LogMessage)));

    m_activeReply = m_replies.first();
    m_activeReply->setRunningState(SoundProviderReply::Running);
    m_worker->get(m_activeReply->request());

    emit changed();
}
