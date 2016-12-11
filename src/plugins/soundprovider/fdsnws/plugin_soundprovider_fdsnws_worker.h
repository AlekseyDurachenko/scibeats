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
#ifndef PLUGIN_SOUNDPROVIDER_FDSNWS_WORKER_H
#define PLUGIN_SOUNDPROVIDER_FDSNWS_WORKER_H


#include "abstractsoundproviderworker.h"


// ----------------------------------------------------------------------
namespace plugin {
namespace soundprovider {
namespace fdsnws {
// ----------------------------------------------------------------------


class Worker : public AbstractSoundProviderWorker
{
    Q_OBJECT
public:
    explicit Worker(QNetworkAccessManager *network, QObject *parent = 0);

protected slots:
    virtual void doGet() final;
    virtual void doAbort() final;
};


// ----------------------------------------------------------------------
} // namespace fdsnws
} // namespace soundprovider
} // namespace plugin
// ----------------------------------------------------------------------


#endif // PLUGIN_SOUNDPROVIDER_FDSNWS_WORKER_H
