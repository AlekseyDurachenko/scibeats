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
#ifndef SOUNDWRITERREPLY_H
#define SOUNDWRITERREPLY_H


#include "soundwriterresultstate.h"
#include "soundwriterrequest.h"
#include <QObject>


class SoundWriterReply : public QObject
{
    Q_OBJECT
    friend class Test_SoundWriterReply;
    friend class SoundWriter;
public:
    enum RunningState {
        Queue,
        Running,
        Aborting,
        Finished
    };

private:
    explicit SoundWriterReply(QObject *parent = 0);
    void setRequest(const SoundWriterRequest &request);
    void setResultState(SoundWriterResultState state);
    void setResultMessage(const QString &message);
    void setRunningState(RunningState state);
    void setProgress(double value, double total);
    void finish();

public:
    inline const SoundWriterRequest &request() const;
    inline SoundWriterResultState resultState() const;
    inline const QString &resultMessage() const;
    inline RunningState runningState() const;

signals:
    void aboutToBeAborted();
    void progress(double value, double total);
    void finished();

public slots:
    void abort();

private:
    SoundWriterRequest m_request;
    SoundWriterResultState m_resultState;
    QString m_resultMessage;
    RunningState m_runningState;
};

const SoundWriterRequest &SoundWriterReply::request() const
{
    return m_request;
}

SoundWriterResultState SoundWriterReply::resultState() const
{
    return m_resultState;
}

const QString &SoundWriterReply::resultMessage() const
{
    return m_resultMessage;
}

SoundWriterReply::RunningState SoundWriterReply::runningState() const
{
    return m_runningState;
}


#endif // SOUNDWRITERREPLY_H
