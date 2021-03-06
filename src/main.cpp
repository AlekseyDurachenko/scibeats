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
#include <QApplication>
#include "plugins/playbackend/plugin_playbackend_factory.h"
#include "plugins/datasource/plugin_datasource_factory.h"
#include "cmainwindow.h"
#include "csound.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    plugin::playbackend::Factory::init();
    plugin::datasource::Factory::init();

    qRegisterMetaType<CSound>("CSound");
    qRegisterMetaType<QseGeometry>("QseGeometry");
    qRegisterMetaType<QList<float> >("QList<float>");

    CMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
