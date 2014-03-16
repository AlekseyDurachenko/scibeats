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
#include "csndfile.h"


CSndFile::CSndFile(QIODevice *io)
{
    m_io = io;
    m_snd = 0;

    m_vioPtr.get_filelen = &qsf_vio_get_filelen;
    m_vioPtr.read = &CSndFile::qsf_vio_read;
    m_vioPtr.seek = &CSndFile::qsf_vio_seek;
    m_vioPtr.tell = &CSndFile::qsf_vio_tell;
    m_vioPtr.write = &CSndFile::qsf_vio_write;

    memset(&m_info, 0, sizeof(m_info));
}

CSndFile::~CSndFile()
{
    close();
}

bool CSndFile::open(int mode)
{
    if (m_io)
    {
        m_snd = sf_open_virtual(&m_vioPtr, mode, &m_info, m_io);
        return (m_snd != 0);
    }
    return false;
}

bool CSndFile::open(QIODevice *io, int mode)
{
    if (io)
    {
        m_snd = sf_open_virtual(&m_vioPtr, mode, &m_info, io);
        return (m_snd != 0);
    }
    return false;
}

sf_count_t CSndFile::readf_float(float *ptr, sf_count_t frames)
{
    return sf_readf_float(m_snd, ptr, frames);
}


sf_count_t CSndFile::writef_float(const float *ptr, sf_count_t frames)
{
    return sf_writef_float(m_snd, ptr, frames);
}

bool CSndFile::close()
{
    return (sf_close(m_snd) == 0);
}

QString CSndFile::getString(int id)
{
    return QString::fromUtf8(sf_get_string(m_snd, id));
}

void CSndFile::setString(int id, const QString &value)
{
    sf_set_string(m_snd, id, value.toUtf8().data());
}

void CSndFile::setFrames(sf_count_t frames)
{
    m_info.frames = frames;
}

void CSndFile::setSampleRate(int sampleRate)
{
    m_info.samplerate = sampleRate;
}

void CSndFile::setChannels(int channels)
{
    m_info.channels = channels;
}

void CSndFile::setFormat(int format)
{
    m_info.format = format;
}

sf_count_t CSndFile::qsf_vio_get_filelen(void *user_data)
{
    return reinterpret_cast<QIODevice*>(user_data)->size();
}

sf_count_t CSndFile::qsf_vio_seek(sf_count_t offset,
        int whence, void *user_data)
{
    sf_count_t pos = 0;
    switch (whence)
    {
        case SEEK_SET :
            pos = offset;
            break;
        case SEEK_CUR :
            pos = reinterpret_cast<QIODevice*>(user_data)->pos() + offset;
            break;
        case SEEK_END :
            pos = reinterpret_cast<QIODevice*>(user_data)->size() - offset;
            break;
        default :
            pos = 0;
            break;
    }
    return reinterpret_cast<QIODevice*>(user_data)->seek(pos);
}

sf_count_t CSndFile::qsf_vio_read(void *ptr, sf_count_t count, void *user_data)
{
    return reinterpret_cast<QIODevice*>(user_data)->read(
            reinterpret_cast<char*>(ptr), count);
}

sf_count_t CSndFile::qsf_vio_write(const void *ptr,
        sf_count_t count, void *user_data)
{
    return reinterpret_cast<QIODevice*>(user_data)->write(
            reinterpret_cast<const char*>(ptr), count);
}

sf_count_t CSndFile::qsf_vio_tell(void *user_data)
{
    return reinterpret_cast<QIODevice*>(user_data)->pos();
}
