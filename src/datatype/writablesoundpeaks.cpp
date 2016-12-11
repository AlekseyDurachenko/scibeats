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
#include "writablesoundpeaks.h"
#include <cstring>
#include <QDebug>


// compute the minimums and maximums for samples for two samples per pixel
template<class T>
static void computeLevelTwo(const T *samples, T *minimums, T *maximums, qint64 count)
{
    const qint64 half = count / 2;
    for (qint64 i = 0; i < half; ++i) {
        const T &a = samples[i * 2];
        const T &b = samples[i * 2 + 1];
        if (a >= b) {
            minimums[i] = b;
            maximums[i] = a;
        }
        else {
            minimums[i] = a;
            maximums[i] = b;
        }
    }

    if (count % 2) {
        minimums[half] = maximums[half] = samples[count - 1];
    }
}


// compress minimums: zoom level N to zoom level N*2 (N samples per pixel)
template<class T>
static void compressMinimums(const T *src, T *dest, qint64 count)
{
    const qint64 half = count / 2;
    for (qint64 i = 0; i < half; ++i) {
        const T &a = src[i * 2];
        const T &b = src[i * 2 + 1];
        dest[i] = qMin(a, b);
    }

    if (count % 2) {
        dest[half] = src[count - 1];
    }
}


// compress maximums: zoom level N to zoom level N*2 (N samples per pixel)
template<class T>
static void compressMaximums(const T *src, T *dest, qint64 count)
{
    const qint64 half = count / 2;
    for (qint64 i = 0; i < half; ++i) {
        const T &a = src[i * 2];
        const T &b = src[i * 2 + 1];
        dest[i] = qMax(a, b);
    }

    if (count % 2) {
        dest[half] = src[count - 1];
    }
}


WritableSoundPeaks::WritableSoundPeaks()
    : data(nullptr)
{
}

WritableSoundPeaks::~WritableSoundPeaks()
{
    delete data;
}

bool WritableSoundPeaks::create(const Sound &sound, QAtomicInt *abort, QString *reason)
{
    if (sound.isNull()) {
        clear();
        return true;
    }

    // allocate the SoundData object if not exists
    if (!data) {
        data = new SoundPeaksData;
    }

    // calculate the array size for each spp
    qint64 spp = 2;
    qint64 cnt = sound.sampleCount() / 2 + (sound.sampleCount() % 2 ? 1 : 0);
    qint64 totalCnt = 0;
    while (cnt > 0) {
        data->peakCount[spp] = cnt;
        if (spp == 2) {
            for (int i = 0; i < sound.channelCount(); ++i) {
                data->minOffset[i][2] = i * sound.channelCount() * cnt;
                data->maxOffset[i][2] = i * sound.channelCount() * cnt + cnt;
            }
        }

        if (cnt < 2) {
            break;
        }

        totalCnt += cnt * sound.channelCount() * 2;
        spp *= 2;
        cnt = cnt / 2 + (cnt % 2 ? 1 : 0);

        if (cnt > 0) {
            const qint64 offset = data->minOffset[0][spp / 2] + data->peakCount[spp / 2] * sound.channelCount() * 2;
            for (int i = 0; i < sound.channelCount(); ++i) {
                data->minOffset[i][spp] = offset + i * sound.channelCount() * cnt;
                data->maxOffset[i][spp] = offset + i * sound.channelCount() * cnt + cnt;
            }
        }
    }

    try {
        // unmap the previous mapping if mapped
        if (data->peaks) {
            if (!data->file->unmap(reinterpret_cast<uchar *>(data->peaks))) {
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
        if (!data->file->resize(totalCnt * sizeof(float))) {
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
        data->peaks = reinterpret_cast<float *>(data->file->map(0, data->file->size()));
        if (!data->peaks) {
            throw QObject::tr("can't map the temporary file \"%1\"")
            .arg(data->file->fileName());
        }

        // A file should be open for a map to succeed but the file
        // does not need to stay open after the memory has been mapped.
        // (see QFile::map())
        data->file->close();

        for (int ch = 0; ch < sound.channelCount(); ++ch) {
            qint64 spp = 2;
            while (data->peakCount.contains(spp)) {
                // abort if needed
                if (abort && *abort) {
                    throw QObject::tr("aborted");
                }

                float *minimums = &data->peaks[data->calcMinPeakIndex(ch, spp)];
                float *maximums = &data->peaks[data->calcMaxPeakIndex(ch, spp)];
                if (spp == 2) {
                    computeLevelTwo(sound.samples(ch), minimums, maximums, sound.sampleCount());
                }
                else {
                    const float *prevMinimums = &data->peaks[data->calcMinPeakIndex(ch, spp/2)];
                    const float *prevMaximums = &data->peaks[data->calcMaxPeakIndex(ch, spp/2)];
                    const qint64 prevCount = data->calcPeakCount(spp/2);
                    compressMinimums(prevMinimums, minimums, prevCount);
                    compressMaximums(prevMaximums, maximums, prevCount);
                }

                spp *= 2;
                cnt = cnt / 2 + (cnt % 2 ? 1 : 0);
            }
        }

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

void WritableSoundPeaks::clear()
{

    delete data;
    data = nullptr;
}

/*!
 * \brief WritableSound::toSound
 *        creates immutable Sound object from current state
 *
 * \note after calling this method, the current state of the object will be
 *       empty because the state is transferred as a whole, without copying
 */
SoundPeaks WritableSoundPeaks::toSoundPeaks()
{
    SoundPeaks soundPeaks(data);
    data = nullptr;
    return soundPeaks;
}
