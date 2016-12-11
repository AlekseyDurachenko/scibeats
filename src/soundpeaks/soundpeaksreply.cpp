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
#include "soundpeaksreply.h"


SoundPeaksReply::SoundPeaksReply(QObject *parent)
    : QObject(parent)
{
    m_resultState = SoundPeaksResultState::Success;
    m_runningState = Queue;
}

void SoundPeaksReply::setRequest(const SoundPeaksRequest &request)
{
    m_request = request;
}

void SoundPeaksReply::setResultState(SoundPeaksResultState state)
{
    m_resultState = state;
}

void SoundPeaksReply::setResultMessage(const QString &message)
{
    m_resultMessage = message;
}

void SoundPeaksReply::setSoundPeaks(const SoundPeaks &soundPeaks)
{
    m_soundPeaks = soundPeaks;
}

void SoundPeaksReply::setRunningState(SoundPeaksReply::RunningState state)
{
    m_runningState = state;
}

void SoundPeaksReply::setProgress(double value, double total)
{
    emit progress(value, total);
}

void SoundPeaksReply::finish()
{
    emit finished();
}

void SoundPeaksReply::abort()
{
    emit aboutToBeAborted();
}
