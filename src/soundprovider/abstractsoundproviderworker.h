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
#ifndef ABSTRACTSOUNDPROVIDERWORKER_H
#define ABSTRACTSOUNDPROVIDERWORKER_H


#include "sound.h"
#include "soundtags.h"
#include "logmessage.h"
#include "soundproviderresultstate.h"
#include "soundproviderrequest.h"
#include <QObject>
#include <QNetworkAccessManager>


class AbstractSoundProviderWorker : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSoundProviderWorker(QNetworkAccessManager *network, QObject *parent = 0);

    inline bool isFinished() const;
    inline bool isRunning() const;
    inline bool isAborted() const;

    inline QNetworkAccessManager *network() const;
    inline const SoundProviderRequest &request() const;

    void get(const SoundProviderRequest &request);
    void abort();

signals:
    void progress(double value, double total);
    void progress(const LogMessage &logMessage);
    void finished(SoundProviderResultState state,
                  const Sound &sound,
                  const SoundTags &tags,
                  const QString &message);

protected:
    void setProgress(double value, double total);
    void setProgress(const LogMessage &logMesasge);
    void setFinished(SoundProviderResultState state,
                     const Sound &sound = Sound(),
                     const SoundTags &tags = SoundTags(),
                     const QString &message = QString());

protected slots:
    void doGet();
    void doAbort();

private:
    QNetworkAccessManager *m_network;
    SoundProviderRequest m_request;
    bool m_running;
    bool m_aborted;
};

bool AbstractSoundProviderWorker::isFinished() const
{
    return !isRunning();
}

bool AbstractSoundProviderWorker::isRunning() const
{
    return m_running;
}

bool AbstractSoundProviderWorker::isAborted() const
{
    return m_aborted;
}

QNetworkAccessManager *AbstractSoundProviderWorker::network() const
{
    return m_network;
}

const SoundProviderRequest &AbstractSoundProviderWorker::request() const
{
    return m_request;
}

#endif // ABSTRACTSOUNDPROVIDERWORKER_H
