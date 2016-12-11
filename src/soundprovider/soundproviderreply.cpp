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
#include "soundproviderreply.h"


SoundProviderReply::SoundProviderReply(QObject *parent)
    : QObject(parent)
{
    m_resultState = SoundProviderResultState::Success;
    m_runningState = Queue;
}

void SoundProviderReply::setRequest(const SoundProviderRequest &request)
{
    m_request = request;
}

void SoundProviderReply::setResultState(SoundProviderResultState state)
{
    m_resultState = state;
}

void SoundProviderReply::setResultMessage(const QString &message)
{
    m_resultMessage = message;
}

void SoundProviderReply::setSound(const Sound &sound)
{
    m_sound = sound;
}

void SoundProviderReply::setTags(const SoundTags &tags)
{
    m_tags = tags;
}

void SoundProviderReply::setRunningState(SoundProviderReply::RunningState state)
{
    m_runningState = state;
}

void SoundProviderReply::setProgress(double value, double total)
{
    emit progress(value, total);
}

void SoundProviderReply::setProgress(const LogMessage &logMessage)
{
    emit progress(logMessage);
}

void SoundProviderReply::finish()
{
    emit finished();
}

void SoundProviderReply::abort()
{
    emit aboutToBeAborted();
}
