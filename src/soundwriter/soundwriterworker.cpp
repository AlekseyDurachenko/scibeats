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
#include "soundwriterworker.h"
#include <sndfile.h>
#include <iostream>
#include <QFileInfo>


static sf_count_t vio_get_filelen(void *user_data)
{
    return static_cast<QIODevice *>(user_data)->size();
}

static sf_count_t vio_seek(sf_count_t offset, int whence, void *user_data)
{
    sf_count_t pos = 0;
    switch (whence) {
    case SEEK_SET :
        pos = offset;
        break;
    case SEEK_CUR :
        pos = static_cast<QIODevice *>(user_data)->pos() + offset;
        break;
    case SEEK_END :
        pos = static_cast<QIODevice *>(user_data)->size() - offset;
        break;
    default :
        pos = 0;
        break;
    }

    return static_cast<QIODevice *>(user_data)->seek(pos);
}

static sf_count_t vio_read(void *ptr, sf_count_t count, void *user_data)
{
    return static_cast<QIODevice *>(user_data)->read(static_cast<char *>(ptr), count);
}

static sf_count_t vio_write(const void *ptr, sf_count_t count, void *user_data)
{
    return static_cast<QIODevice *>(user_data)->write(static_cast<const char *>(ptr), count);
}

static sf_count_t vio_tell(void *user_data)
{
    return static_cast<QIODevice *>(user_data)->pos();
}


static int file_format_to_sf_format(SoundFileFormat format)
{
    switch (format) {
    case SoundFileFormat::WavUInt8:
        return SF_FORMAT_WAV | SF_FORMAT_PCM_U8;
    case SoundFileFormat::WavInt16:
        return SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    case SoundFileFormat::WavInt24:
        return SF_FORMAT_WAV | SF_FORMAT_PCM_24;
    case SoundFileFormat::WavInt32:
        return SF_FORMAT_WAV | SF_FORMAT_PCM_32;
    case SoundFileFormat::WavFloat:
        return SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    default:
        Q_ASSERT_X(false, "file_format_to_sf_format()", "unknow sound file format");
    }

    return 0;
}

SoundWriterWorker::SoundWriterWorker(QObject *parent)
    : QObject(parent)
{
    m_running = 0;
    m_aborted = 0;
}

bool SoundWriterWorker::isFinished() const
{
    return !isRunning();
}

bool SoundWriterWorker::isRunning() const
{
    return m_running;
}

void SoundWriterWorker::write(const SoundWriterRequest &request)
{
    QMetaObject::invokeMethod(this,
                              "privateWrite",
                              Qt::QueuedConnection,
                              Q_ARG(SoundWriterRequest, request));
}

void SoundWriterWorker::abort()
{
    m_aborted = 1;
}

void SoundWriterWorker::privateWrite(const SoundWriterRequest &request)
{
    if (m_aborted) {
        emit finished(SoundWriterResultState::Aborted, tr("aborted"));
        return;
    }

    emit progress(0.0, 1.0);
    m_aborted = 0;
    m_running = 1;
    QString errorString;
    bool retVal = privateWrite(request, errorString);
    m_running = 0;
    emit progress(1.0, 1.0);

    // on success
    if (retVal) {
        emit finished(SoundWriterResultState::Success, QString());
        return;
    }

    // on abort
    if (m_aborted) {
        emit finished(SoundWriterResultState::Aborted, tr("aborted"));
        return;
    }

    // on any other error
    emit finished(SoundWriterResultState::Failed, errorString);
}

bool SoundWriterWorker::privateWrite(const SoundWriterRequest &request, QString &reason)
{
    Q_ASSERT(request.isNull() == false);
    Q_ASSERT(qRound(request.fileSampleRate()) > 0);
    Q_ASSERT(qRound(request.soundSampleRate()) > 0);
    Q_ASSERT(request.sound().sampleCount() > 0);
    Q_ASSERT(request.sound().channelCount() >= 1
             && request.sound().channelCount() <= 2);

    // try to open file for write the wav file
    QFile file(request.fileName());
    if (!file.open(QIODevice::WriteOnly)) {
        reason = file.errorString();
        return false;
    }

    // aliases
    const Sound &sound = request.sound();
    const SoundTags &tags = request.tags();

    SF_VIRTUAL_IO vio;
    vio.get_filelen = &vio_get_filelen;
    vio.read = &vio_read;
    vio.seek = &vio_seek;
    vio.tell = &vio_tell;
    vio.write = &vio_write;

    SF_INFO info;
    info.frames = sound.sampleCount();
    info.samplerate = qRound(request.fileSampleRate());
    info.channels = sound.channelCount();
    info.sections = 0;
    info.seekable = 0;
    info.format = file_format_to_sf_format(request.fileFormat());

    try {
        // open sound file for write
        SNDFILE *sndfile = sf_open_virtual(&vio, SFM_WRITE, &info, &file);
        if (sndfile == NULL) {
            if (file.errorString().isEmpty()) {
                throw QString(sf_strerror(sndfile));
            }

            throw file.errorString();
        }

        // write the metadata informat-lsndfileion
        sf_set_string(sndfile, SF_STR_TITLE, tags.title().toUtf8().data());
        sf_set_string(sndfile, SF_STR_COPYRIGHT, tags.copyright().toUtf8().data());
        sf_set_string(sndfile, SF_STR_SOFTWARE, tags.software().toUtf8().data());
        sf_set_string(sndfile, SF_STR_ARTIST, tags.artist().toUtf8().data());
        sf_set_string(sndfile, SF_STR_COMMENT, tags.comment().toUtf8().data());
        sf_set_string(sndfile, SF_STR_DATE, tags.date().toUtf8().data());
        sf_set_string(sndfile, SF_STR_GENRE, tags.genre().toUtf8().data());


        // calculate buffers size
        const qint64 frameCount = 65536; // 64krames
        const int channelCount = sound.channelCount();

        // create the buffers
        float *const buffer = new float[frameCount * channelCount];
        const float **const chs = new const float * [channelCount];

        // we write data by blocks
        for (qint64 pos = 0; pos < sound.sampleCount(); pos += frameCount) {
            // calculate the actual number of frames for block
            qint64 actualFrameCount = frameCount;
            if (pos + actualFrameCount > sound.sampleCount()) {
                actualFrameCount = sound.sampleCount() - pos;
            }

            // assign the pointers to the channels
            float *buffer_tmp = buffer;
            for (int ch = 0; ch < sound.channelCount(); ++ch) {
                chs[ch] = sound.samples(ch, pos);
            }

            // multiplex the channels to the buffer
            for (qint64 sample = 0; sample < actualFrameCount; ++sample) {
                for (int ch = 0; ch < sound.channelCount(); ++ch) {
                    *buffer_tmp++ = *chs[ch]++;
                }
            }

            // write to the file
            if (sf_writef_float(sndfile, buffer, actualFrameCount) != actualFrameCount) {
                if (file.errorString().isEmpty()) {
                    throw QString(sf_strerror(sndfile));
                }

                delete []chs;
                delete []buffer;
                sf_close(sndfile);

                throw file.errorString();
            }

            if (m_aborted) {
                delete []chs;
                delete []buffer;
                sf_close(sndfile);

                throw tr("aborted");
            }

            emit progress((1.0 * pos) / sound.sampleCount(), 1.0);
        }

        delete []chs;
        delete []buffer;
        sf_close(sndfile);

        return true;
    }
    catch (const QString &errorString) {
        reason = errorString;
    }

    // before remove the file we should close it
    file.close();

    // if process will aborted or failed we should remove the file
    if (QFile(request.fileName()).exists()) {
        if (!QFile(request.fileName()).remove()) {
            std::cerr << "file \""
                      << QFileInfo(request.fileName()).absoluteFilePath().toStdString()
                      << "\" cannot be removed"
                      << std::endl;
        }
    }

    return false;
}
