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
#include "soundpeaksworker.h"
#include "writablesoundpeaks.h"
#include <unistd.h>
#include <QDebug>


SoundPeaksWorker::SoundPeaksWorker(QObject *parent)
    : QObject(parent)
{
    m_running = 0;
    m_aborted = 0;
}

bool SoundPeaksWorker::isFinished() const
{
    return !isRunning();
}

bool SoundPeaksWorker::isRunning() const
{
    return m_running;
}

void SoundPeaksWorker::write(const SoundPeaksRequest &request)
{
    QMetaObject::invokeMethod(this,
                              "privateWrite",
                              Qt::QueuedConnection,
                              Q_ARG(SoundPeaksRequest, request));
}

void SoundPeaksWorker::abort()
{
    m_aborted = 1;
}

void SoundPeaksWorker::privateWrite(const SoundPeaksRequest &request)
{
    if (m_aborted) {
        emit finished(SoundPeaksResultState::Aborted, SoundPeaks(), tr("aborted"));
        return;
    }

    emit progress(0.0, 1.0);
    m_aborted = 0;
    m_running = 1;
    QString errorString;
    WritableSoundPeaks writablePeaks;
    bool retVal = writablePeaks.create(request.sound(), &m_aborted, &errorString);
    m_running = 0;
    emit progress(1.0, 1.0);

    // on success
    if (retVal) {
        emit finished(SoundPeaksResultState::Success, writablePeaks.toSoundPeaks(), QString());
        return;
    }

    // on abort
    if (m_aborted) {
        emit finished(SoundPeaksResultState::Aborted, SoundPeaks(), tr("aborted"));
        return;
    }

    // on any other error
    emit finished(SoundPeaksResultState::Failed, SoundPeaks(), errorString);
}
