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
#ifndef SOUNDPROVIDER_H
#define SOUNDPROVIDER_H


#include "abstractsoundproviderworker.h"
#include "soundproviderreply.h"
#include <QObject>
#include <QNetworkAccessManager>


class SoundProvider : public QObject
{
    Q_OBJECT
public:
    SoundProvider(QNetworkAccessManager *network, QObject *parent = 0);
    virtual ~SoundProvider() final;

    QNetworkAccessManager *network() const;

    int activeCount() const;
    int queuedCount() const;
    int totalCount() const;

    SoundProviderReply *get(const SoundProviderRequest &request, QString *reason = 0);

signals:
    void changed();

private slots:
    void reply_aboutToBeAborted();
    void reply_destroyed(QObject *obj);
    void worker_progress(double value, double total);
    void worker_progress(const LogMessage &logMessage);
    void worker_finished(SoundProviderResultState state,
                         const Sound &sound,
                         const SoundTags &tags,
                         const QString &message);
    void next();

private:
    QNetworkAccessManager *m_network;
    AbstractSoundProviderWorker *m_worker;
    SoundProviderReply *m_activeReply;
    QList<SoundProviderReply *> m_replies;
};


#endif // SOUNDPROVIDER_H
