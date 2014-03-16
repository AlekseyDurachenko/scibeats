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
#include "plugin_playbackend_portaudio_cplaybackend.h"
#include <portaudio.h>
#include <samplerate.h>
#include <limits>
#include <QDebug>

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
namespace portaudio {
// ----------------------------------------------------------------------


int paPlayCallback(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags, void *userData)
{
    return reinterpret_cast<CPlayBackend*>(userData)->cbPlay(inputBuffer,
        outputBuffer, framesPerBuffer, timeInfo, statusFlags);
}

void paStreamFinished(void* userData)
{
    reinterpret_cast<CPlayBackend*>(userData)->cbFinish();
}

CPlayBackend::CPlayBackend(QObject *parent) :
    CAbstractPlayBackend(parent)
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        qWarning() << Pa_GetErrorText(err);
    }

    m_first = -1;
    m_last = -1;
    m_index = -1;
    m_volume = volumeToInt(1.0);
    m_speed = speedToInt(1.0);
    m_loop = false;
    m_paused = false;
    m_stream = 0;

    m_srcInData = new float[1000000];
    m_srcState = src_new (SRC_LINEAR, 1, 0);
    src_reset(m_srcState);

    setConfiguration(QVariantMap());
}

CPlayBackend::~CPlayBackend()
{
    PaError err = Pa_Terminate();
    if (err != paNoError)
    {
        qWarning() << Pa_GetErrorText(err);
    }

    Pa_CloseStream(m_stream);

    delete [] m_srcInData;
}

bool CPlayBackend::isPlayed() const
{
    return (Pa_IsStreamActive(m_stream) == 1);
}

bool CPlayBackend::isStoped() const
{
    return !isPlayed();
}

bool CPlayBackend::isPaused() const
{
    return isPlayed() && m_paused;
}

bool CPlayBackend::isLooped() const
{
    return m_loop;
}

qint64 CPlayBackend::index() const
{
    return m_index;
}

qint64 CPlayBackend::first() const
{
    return m_first;
}

qint64 CPlayBackend::last() const
{
    return m_last;
}

double CPlayBackend::volume() const
{
    return volumeFromInt(m_volume);
}

double CPlayBackend::speed() const
{
    return speedFromInt(m_speed);
}

const CSound &CPlayBackend::sound() const
{
    return m_sound;
}

const QVariant &CPlayBackend::configuration() const
{
    return m_configuration;
}

int CPlayBackend::sampleRate() const
{
    return 44100;
}

void CPlayBackend::play()
{
    if (m_paused)
    {
        m_paused = false;
        emit played();
    }
    else if (!isPlayed())
    {
        // when playing throws by the paComplite(paAbort) we should correct
        // call the Pa_StopStream, otherwise we cannot start plaing the stream
        // again
        if (Pa_IsStreamActive(m_stream) == 0 && Pa_IsStreamStopped(m_stream) == 0)
        {
            PaError err = Pa_StopStream(m_stream);
            if (err != paNoError)
            {
                qWarning() << Pa_GetErrorText(err);
                return;
            }
        }

        PaError err = Pa_StartStream(m_stream);
        if (err != paNoError)
        {
            qWarning() << Pa_GetErrorText(err);
            return;
        }

        emit played();
    }
}

void CPlayBackend::stop()
{
    if (isPlayed())
    {
        PaError err = Pa_StopStream(m_stream);
        if (err != paNoError)
        {
            qWarning() << Pa_GetErrorText(err);
            return;
        }

        src_reset(m_srcState);

        emit stoped();
    }
}

void CPlayBackend::pause()
{
    if (isPlayed())
    {
        if (m_paused)
        {
            m_paused = false;
            emit played();
        }
        else
        {
            m_paused = true;
            emit paused();
        }
    }
}

void CPlayBackend::setLoop(bool loop)
{
    if (m_loop != loop)
    {
        m_loop = loop;
        emit loopStateChanged(loop);
    }
}

void CPlayBackend::setIndex(qint64 index)
{
    if (m_index != static_cast<int>(index))
    {
        m_index = index;
        emit indexChanged(index);
    }
}

void CPlayBackend::setRange(qint64 first, qint64 last)
{
    if (m_first != static_cast<int>(first) || m_last != static_cast<int>(last))
    {
        m_first = first;
        m_last = last;
        emit rangeChanged(first, last);
    }
}

void CPlayBackend::setVolume(double volume)
{
    int newVolume = volumeToInt(volume);
    if (m_volume != newVolume)
    {
        m_volume = newVolume;
        emit volumeChanged(volume);
    }
}

void CPlayBackend::setSpeed(double speed)
{
    int newSpeed = speedToInt(speed);
    if (m_speed != newSpeed)
    {
        m_speed = newSpeed;
        emit speedChanged(speed);
    }
}

void CPlayBackend::setSound(const CSound &sound)
{
    if (m_sound != sound)
    {
        stop();
        m_sound = sound;
        emit soundChanged(sound);
    }
}

void CPlayBackend::setConfiguration(const QVariant &configuration)
{
    if (m_configuration != configuration)
    {
        PaStreamParameters output;
        output.channelCount = 1;
        output.sampleFormat = paFloat32;
        output.device = Pa_GetDefaultOutputDevice();
        output.suggestedLatency = Pa_GetDeviceInfo(output.device)->defaultLowOutputLatency;
        output.hostApiSpecificStreamInfo = 0;

        if (m_stream)
        {
            PaError err = Pa_CloseStream(m_stream);
            m_stream = 0;
            if (err != paNoError)
            {
                qWarning() << Pa_GetErrorText(err);
            }
        }

        PaError err = Pa_OpenStream(&m_stream, 0, &output, 44100, 4096,
            paClipOff, &paPlayCallback, this);

        if (err != paNoError)
        {
            qWarning() << Pa_GetErrorText(err);
        }

        err = Pa_SetStreamFinishedCallback(m_stream, &paStreamFinished);
        if (err != paNoError)
        {
            qWarning() << Pa_GetErrorText(err);
        }

        // TODO
        emit configuraiotnChanged(configuration);
    }
}

int CPlayBackend::cbPlay(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags)
{
    Q_UNUSED(inputBuffer);
    Q_UNUSED(timeInfo);
    Q_UNUSED(statusFlags);

    float *soundFrames = m_sound.frames();
    double volume = volumeFromInt(m_volume);
    double speed = speedFromInt(m_speed);
    //qint64 first = m_first;
    //qint64 last = m_last;
    //qint64 index = m_index;
    bool looped = m_loop;
    bool paused = m_paused;
    float *output = reinterpret_cast<float *>(outputBuffer);

    if (paused)
    {
        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            output[i] = 0.0;
        }
    }
    else
    {    
        for (unsigned long i = 0; i < framesPerBuffer*speed; ++i)
        {
            if (m_index > m_last)
            {
                if (looped)
                {
                    m_index = m_first;
                }
                else
                {
                    return paComplete;
                }
            }
            m_srcInData[i] = soundFrames[m_index.fetchAndAddOrdered(1)]*volume;
        }

        SRC_DATA src_data;
        src_data.data_in = m_srcInData;
        src_data.input_frames = framesPerBuffer*speed;
        src_data.data_out = output;
        src_data.output_frames = framesPerBuffer;
        src_data.end_of_input = 0;
        src_data.src_ratio = 1.0/speed;
        src_set_ratio (m_srcState, 1.0/speed);
        src_process(m_srcState, &src_data);
    }

    return paContinue;
}

void CPlayBackend::cbFinish()
{
    src_reset(m_srcState);
    emit stoped();
}

double CPlayBackend::volumeFromInt(int volume) const
{
    return (1.0 * volume) / std::numeric_limits<int>::max();
}

int CPlayBackend::volumeToInt(double volume) const
{
    return static_cast<int>(volume * std::numeric_limits<int>::max());
}

double CPlayBackend::speedFromInt(int speed) const
{
    if (speed >= 100)
    {
        return (1.0 + (speed-100)/10.0);
    }
    else
    {
        return (speed / 100.0);
    }
}

int CPlayBackend::speedToInt(double speed) const
{
    if (speed >= 1.0)
    {
        return static_cast<int>(100+(speed-1.0)*10.0);
    }
    else
    {
        return static_cast<int>(speed*100);
    }
}

// ----------------------------------------------------------------------
} // namespace portaudio
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------
