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
#include "writablesound.h"
#include <cstring>


WritableSound::WritableSound()
    : data(nullptr)
{
}

WritableSound::~WritableSound()
{
    delete data;
}

bool WritableSound::create(int channelCount, qint64 sampleCount, QString *reason)
{
    Q_ASSERT(channelCount >= 0);
    Q_ASSERT(sampleCount >= 0);

    if (channelCount < 1 || channelCount > 2) {
        if (reason) {
            *reason = QObject::tr("the number of channels must be in the range from 1 to 2");
        }
        return false;
    }

    // allocate the SoundData object if not exists
    if (!data) {
        data = new SoundData;
    }

    try {
        // unmap the previous mapping if mapped
        if (data->samples) {
            if (!data->file->unmap(reinterpret_cast<uchar *>(data->samples))) {
                // TODO: what we should do if memory cannot be unmapped?
            }
        }

        // create the temporary file if not created
        if (!data->file->isOpen() && !data->file->open()) {
            throw QObject::tr("can't create the temporary file \"%1\": %2")
            .arg(data->file->fileName())
            .arg(data->file->errorString());
        }

        // resize the temporary file
        const std::size_t arraySize = channelCount * sampleCount * sizeof(float);
        if (!data->file->resize(arraySize)) {
            throw QObject::tr("can't resize the temporary file \"%1\": %2")
            .arg(data->file->fileName())
            .arg(data->file->errorString());
        }

        // data->samples = data->file->map(0, arraySize);
        // this cause the:
        // QFSFileEngine::map: Mapping a file beyond its size is not portable
        // for solve, we should call the file->size() or file->exists() method
        // [QTBUG-16285]
        // Minimal example:
        // QTemporaryFile file;
        // qDebug() << file.open();
        // qDebug() << file.resize(100);
        // qDebug() << file.map(0, 100);
        // qDebug() << file.resize(200);
        // qDebug() << file.map(0, 200);

        // map the temporary file
        data->samples = reinterpret_cast<float *>(data->file->map(0, data->file->size()));
        if (!data->samples) {
            throw QObject::tr("can't map the temporary file \"%1\"")
            .arg(data->file->fileName());
        }

        // A file should be open for a map to succeed but the file
        // does not need to stay open after the memory has been mapped.
        // (see QFile::map())
        data->file->close();

        data->channelCount = channelCount;
        data->sampleCount = sampleCount;

        return true;
    }
    catch (const QString &error) {
        if (reason) {
            *reason = error;
        }

        // if any error happened clear the object
        clear();
    }

    return false;
}

void WritableSound::clear()
{
    delete data;
    data = nullptr;
}

/*!
 * \brief WritableSound::fromSound
 *        loads full state of immutable Sound object
 *
 * \note previous state will be lost
 */
bool WritableSound::fromSound(const Sound &sound, QString *reason)
{
    if (sound.isNull()) {
        clear();
        return true;
    }

    if (!create(sound.channelCount(), sound.sampleCount(), reason)) {
        return false;
    }

    const std::size_t arraySize = sound.channelCount() * sound.sampleCount() * sizeof(float);
    std::memcpy(data->samples, sound.samples(), arraySize);

    return true;
}

/*!
 * \brief WritableSound::toSound
 *        creates immutable Sound object from current state
 *
 * \note after calling this method, the current state of the object will be
 *       empty because the state is transferred as a whole, without copying
 */
Sound WritableSound::toSound()
{
    Sound sound(data);
    data = nullptr;
    return sound;
}
