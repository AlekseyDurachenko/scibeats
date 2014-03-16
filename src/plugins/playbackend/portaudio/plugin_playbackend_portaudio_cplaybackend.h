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
#ifndef PLUGIN_PLAYBACKEND_PORTAUDIO_CPLAYBACK_H
#define PLUGIN_PLAYBACKEND_PORTAUDIO_CPLAYBACK_H

#include "../plugin_playbackend_cabstractplaybackend.h"
#include <portaudio.h>
#include <samplerate.h>

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
namespace portaudio {
// ----------------------------------------------------------------------

class CPlayBackend : public CAbstractPlayBackend
{
    Q_OBJECT
    friend int paPlayCallback(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags, void *userData);
    friend void paStreamFinished(void* userData);
public:
    explicit CPlayBackend(QObject *parent = 0);
    ~CPlayBackend();

    bool isPlayed() const;
    bool isStoped() const;
    bool isPaused() const;
    bool isLooped() const;
    qint64 index() const;
    qint64 first() const;
    qint64 last() const;
    double volume() const;
    double speed() const;
    const CSound &sound() const;
    const QVariant &configuration() const;
    int sampleRate() const;

public slots:
    void play();
    void stop();
    void pause();
    void setLoop(bool loop);
    void setIndex(qint64 index);
    void setRange(qint64 first, qint64 last);
    void setVolume(double volume);
    void setSpeed(double speed);
    void setSound(const CSound &sound);
    void setConfiguration(const QVariant &configuration);

private:
    int cbPlay(const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags);
    void cbFinish();

private:
    double volumeFromInt(int volume) const;
    int volumeToInt(double volume) const;
    double speedFromInt(int speed) const;
    int speedToInt(double speed) const;

private:
    PaStreamParameters m_outputParameters;
    PaStream *m_stream;
    QVariant m_configuration;
    CSound m_sound;
    QAtomicInt m_paused;
    QAtomicInt m_loop;
    QAtomicInt m_first;
    QAtomicInt m_last;
    QAtomicInt m_index;
    QAtomicInt m_volume;
    QAtomicInt m_speed;

    // resample
    SRC_STATE* m_srcState;
    float *m_srcInData;


};

// ----------------------------------------------------------------------
} // namespace portaudio
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_PLAYBACKEND_PORTAUDIO_CPLAYBACK_H
