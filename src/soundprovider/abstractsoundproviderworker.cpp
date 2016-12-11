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
#include "abstractsoundproviderworker.h"
#include <QFileInfo>
#include <QDebug>


AbstractSoundProviderWorker::AbstractSoundProviderWorker(QNetworkAccessManager *network, QObject *parent)
    : QObject(parent)
{
    m_running = false;
    m_aborted = false;
    m_network = network;
}

void AbstractSoundProviderWorker::get(const SoundProviderRequest &request)
{
    Q_ASSERT(!request.isNull());

    if (!m_aborted) {
        m_request = request;
        m_running = true;
        emit progress(0.0, 1.0);
        emit progress(LogMessage(LogMessage::Information, tr("started")));
        QMetaObject::invokeMethod(this, "doGet", Qt::QueuedConnection);
    }
}

void AbstractSoundProviderWorker::abort()
{
    if (!m_aborted) {
        m_aborted = true;
        m_running = false;
        doAbort();
        emit progress(1.0, 1.0);
        emit progress(LogMessage(LogMessage::Information, tr("aborted")));
        emit finished(SoundProviderResultState::Aborted,
                      Sound(),
                      SoundTags(),
                      tr("aborted"));
    }
}

void AbstractSoundProviderWorker::setProgress(double value, double total)
{
    if (!m_aborted) {
        emit progress(value, total);
    }
}

void AbstractSoundProviderWorker::setProgress(const LogMessage &logMesasge)
{
    if (!m_aborted) {
        emit progress(logMesasge);
    }
}

void AbstractSoundProviderWorker::setFinished(SoundProviderResultState state,
                                              const Sound &sound,
                                              const SoundTags &tags,
                                              const QString &message)
{
    if (!m_aborted) {
        m_running = false;
        emit progress(1.0, 1.0);
        emit progress(LogMessage(LogMessage::Information, tr("finished")));
        emit finished(state, sound, tags, message);
    }
}

void AbstractSoundProviderWorker::doGet()
{
    qDebug("call = AbstractSoundProviderWorker::doGet()");
    setFinished(SoundProviderResultState::Success);
}

void AbstractSoundProviderWorker::doAbort()
{
    qDebug("call = AbstractSoundProviderWorker::doAbort()");
}
