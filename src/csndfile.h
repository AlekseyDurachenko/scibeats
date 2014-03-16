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
#ifndef CSNDFILE_H
#define CSNDFILE_H

#include <sndfile.h>
#include <QIODevice>

// This class is libsndfile wrapper for QIODevice backend
class CSndFile
{
public:
    explicit CSndFile(QIODevice *io = 0);
    ~CSndFile();

    bool open(int mode);
    bool open(QIODevice *io, int mode);
    sf_count_t readf_float(float *ptr, sf_count_t frames);
    sf_count_t writef_float(const float *ptr, sf_count_t frames);
    bool close();

    inline sf_count_t frames() const;
    inline int sampleRate() const;
    inline int channels() const;
    inline int format() const;
    inline int sections() const;
    inline int seekable() const;

    QString getString(int id);
    void setString(int id, const QString &value);

    void setFrames(sf_count_t frames);
    void setSampleRate(int sampleRate);
    void setChannels(int channels);
    void setFormat(int format);
private:
    static sf_count_t qsf_vio_get_filelen(void *user_data);
    static sf_count_t qsf_vio_seek(sf_count_t offset, int whence, void *user_data);
    static sf_count_t qsf_vio_read(void *ptr, sf_count_t count, void *user_data);
    static sf_count_t qsf_vio_write(const void *ptr, sf_count_t count, void *user_data);
    static sf_count_t qsf_vio_tell(void *user_data);

private:
    QIODevice *m_io;
    SF_VIRTUAL_IO m_vioPtr;
    SF_INFO m_info;
    SNDFILE *m_snd;

private:
    Q_DISABLE_COPY(CSndFile)

};


inline sf_count_t CSndFile::frames() const
{
    return m_info.frames;
}

inline int CSndFile::sampleRate() const
{
    return m_info.samplerate;
}

inline int CSndFile::channels() const
{
    return m_info.channels;
}

inline int CSndFile::format() const
{
    return m_info.format;
}

inline int CSndFile::sections() const
{
    return m_info.sections;
}

inline int CSndFile::seekable() const
{
    return m_info.seekable;
}


#endif // CSNDFILE_H
