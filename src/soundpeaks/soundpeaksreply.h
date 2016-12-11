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
#ifndef SOUNDPEAKSREPLY_H
#define SOUNDPEAKSREPLY_H


#include "soundpeaksresultstate.h"
#include "soundpeaksrequest.h"
#include <QObject>


class SoundPeaksReply : public QObject
{
    Q_OBJECT
    friend class Test_SoundPeaksReply;
    friend class SoundPeaksCalculator;
public:
    enum RunningState {
        Queue,
        Running,
        Aborting,
        Finished
    };

private:
    explicit SoundPeaksReply(QObject *parent = 0);
    void setRequest(const SoundPeaksRequest &request);
    void setResultState(SoundPeaksResultState state);
    void setResultMessage(const QString &message);
    void setSoundPeaks(const SoundPeaks &soundPeaks);
    void setRunningState(RunningState state);
    void setProgress(double value, double total);
    void finish();

public:
    inline const SoundPeaksRequest &request() const;
    inline const SoundPeaks &soundPeaks() const;
    inline SoundPeaksResultState resultState() const;
    inline const QString &resultMessage() const;
    inline RunningState runningState() const;

signals:
    void aboutToBeAborted();
    void progress(double value, double total);
    void finished();

public slots:
    void abort();

private:
    SoundPeaksRequest m_request;
    SoundPeaksResultState m_resultState;
    SoundPeaks m_soundPeaks;
    QString m_resultMessage;
    RunningState m_runningState;
};

const SoundPeaksRequest &SoundPeaksReply::request() const
{
    return m_request;
}

const SoundPeaks &SoundPeaksReply::soundPeaks() const
{
    return m_soundPeaks;
}

SoundPeaksResultState SoundPeaksReply::resultState() const
{
    return m_resultState;
}

const QString &SoundPeaksReply::resultMessage() const
{
    return m_resultMessage;
}

SoundPeaksReply::RunningState SoundPeaksReply::runningState() const
{
    return m_runningState;
}


#endif // SOUNDPEAKSREPLY_H
