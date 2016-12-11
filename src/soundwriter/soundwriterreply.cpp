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
#include "soundwriterreply.h"


SoundWriterReply::SoundWriterReply(QObject *parent)
    : QObject(parent)
{
    m_resultState = SoundWriterResultState::Success;
    m_runningState = Queue;
}

void SoundWriterReply::setRequest(const SoundWriterRequest &request)
{
    m_request = request;
}

void SoundWriterReply::setResultState(SoundWriterResultState state)
{
    m_resultState = state;
}

void SoundWriterReply::setResultMessage(const QString &message)
{
    m_resultMessage = message;
}

void SoundWriterReply::setRunningState(SoundWriterReply::RunningState state)
{
    m_runningState = state;
}

void SoundWriterReply::setProgress(double value, double total)
{
    emit progress(value, total);
}

void SoundWriterReply::finish()
{
    emit finished();
}

void SoundWriterReply::abort()
{
    emit aboutToBeAborted();
}
