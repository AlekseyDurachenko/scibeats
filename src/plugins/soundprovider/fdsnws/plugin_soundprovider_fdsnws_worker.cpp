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
#include "plugin_soundprovider_fdsnws_worker.h"
#include "plugin_soundprovider_fdsnws_parameters.h"
#include "writablesound.h"
#include <unistd.h>
#include <QDebug>


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
namespace fdsnws {
// ----------------------------------------------------------------------


Worker::Worker(QNetworkAccessManager *network, QObject *parent)
    : AbstractSoundProviderWorker(network, parent)
{
}

void Worker::doGet()
{
    WritableSound writableSound;
    writableSound.create(1, 1000000);
    for (int i = 0; i < 1000000; ++i) {
        //if (!(i % 1000)) setProgress(i, 1000000);
        *writableSound.samples(0, i) = (5000 - qrand() % 10000) / 1000.0;
    }

    Parameters params;
    params.fromXml(request().query());

    //qDebug("call = plugin::soundprovider::fdsnws::Worker::doGet()");
    setFinished(SoundProviderResultState::Success, writableSound.toSound(), params.tags());
}

void Worker::doAbort()
{
    qDebug("call = plugin::soundprovider::fdsnws::Worker::doAbort()");
}


// ----------------------------------------------------------------------
} // namespace fdsnws
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------
