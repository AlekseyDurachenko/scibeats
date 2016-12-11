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
#ifndef SOUNDPROVIDERREPLY_H
#define SOUNDPROVIDERREPLY_H


#include "sound.h"
#include "soundtags.h"
#include "soundproviderresultstate.h"
#include "soundproviderrequest.h"
#include "logmessage.h"
#include <QObject>


class SoundProviderReply : public QObject
{
    Q_OBJECT
    friend class Test_SoundProviderReply;
    friend class SoundProvider;
public:
    enum RunningState {
        Queue,
        Running,
        Aborting,
        Finished
    };

private:
    explicit SoundProviderReply(QObject *parent = 0);
    void setRequest(const SoundProviderRequest &request);
    void setResultState(SoundProviderResultState state);
    void setResultMessage(const QString &message);
    void setSound(const Sound &sound);
    void setTags(const SoundTags &tags);
    void setRunningState(RunningState state);
    void setProgress(double value, double total);
    void setProgress(const LogMessage &logMessage);
    void finish();

public:
    inline const SoundProviderRequest &request() const;
    inline const Sound &sound() const;
    inline const SoundTags &tags() const;
    inline SoundProviderResultState resultState() const;
    inline const QString &resultMessage() const;
    inline RunningState runningState() const;

signals:
    void aboutToBeAborted();
    void progress(double value, double total);
    void progress(const LogMessage &logMessage);
    void finished();

public slots:
    void abort();

private:
    SoundProviderRequest m_request;
    SoundProviderResultState m_resultState;
    Sound m_sound;
    SoundTags m_tags;
    QString m_resultMessage;
    RunningState m_runningState;
};

const SoundProviderRequest &SoundProviderReply::request() const
{
    return m_request;
}

const Sound &SoundProviderReply::sound() const
{
    return m_sound;
}

const SoundTags &SoundProviderReply::tags() const
{
    return m_tags;
}

SoundProviderResultState SoundProviderReply::resultState() const
{
    return m_resultState;
}

const QString &SoundProviderReply::resultMessage() const
{
    return m_resultMessage;
}

SoundProviderReply::RunningState SoundProviderReply::runningState() const
{
    return m_runningState;
}


#endif // SOUNDPROVIDERREPLY_H
