// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef PLUGIN_PLAYBACKEND_CABSTRACTPLAYBACK_H
#define PLUGIN_PLAYBACKEND_CABSTRACTPLAYBACK_H

#include <QObject>
#include "csound.h"

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
// ----------------------------------------------------------------------

class CAbstractPlayBackend : public QObject
{
    Q_OBJECT
public:
    explicit CAbstractPlayBackend(QObject *parent = 0);
    virtual ~CAbstractPlayBackend();

    virtual bool isPlayed() const = 0;
    virtual bool isStoped() const = 0;
    virtual bool isPaused() const = 0;
    virtual bool isLooped() const = 0;
    virtual qint64 index() const = 0;
    virtual qint64 first() const = 0;
    virtual qint64 last() const = 0;
    virtual double volume() const = 0;
    virtual double speed() const = 0;
    virtual const CSound &sound() const = 0;
    virtual const QVariant &configuration() const = 0;
    virtual int sampleRate() const = 0;

public slots:
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void setLoop(bool loop) = 0;
    virtual void setIndex(qint64 index) = 0;
    virtual void setRange(qint64 first, qint64 last) = 0;
    virtual void setVolume(double volume) = 0;
    virtual void setSpeed(double speed) = 0;
    virtual void setSound(const CSound &sound) = 0;
    virtual void setConfiguration(const QVariant &configuration) = 0;

signals:
    void played();
    void stoped();
    void paused();
    void loopStateChanged(bool loop);
    void indexChanged(qint64 index);
    void rangeChanged(qint64 first, qint64 last);
    void volumeChanged(double volume);
    void speedChanged(double speed);
    void soundChanged(const CSound &sound);
    void configuraiotnChanged(const QVariant &configuration);

};


// ----------------------------------------------------------------------
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_PLAYBACKEND_CABSTRACTPLAYBACK_H
