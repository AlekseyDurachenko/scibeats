// Copyright 2015-2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
//#include "mainwindow.h"
#include "resources.h"
#include "theme.h"
#include "soundwriterrequest.h"
#include "soundwriter.h"
#include "soundpeaksresultstate.h"
#include "soundpeaksrequest.h"
#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include <writablesound.h>
#include <QDir>
#include <QDebug>
#include <soundcontrolsampleratewidget.h>
#include <soundcontrolpanelwidget.h>
#include <soundwidget.h>
#include "soundpeakscalculator.h"
#include "soundproviderrequest.h"
#include "logmessage.h"
#include "plugin_soundprovider_factory.h"
#include "sounddatasource.h"


void registerMetaTypes()
{
    qRegisterMetaType<LogMessage>("LogMessage");
    qRegisterMetaType<Sound>("Sound");
    qRegisterMetaType<SoundPeaks>("SoundPeaks");
    qRegisterMetaType<SoundWriterRequest>("SoundWriterRequest");
    qRegisterMetaType<SoundWriterResultState>("SoundWriterResultState");
    qRegisterMetaType<SoundPeaksRequest>("SoundPeaksRequest");
    qRegisterMetaType<SoundPeaksResultState>("SoundPeaksResultState");
    qRegisterMetaType<SoundProviderRequest>("SoundProviderRequest");
}

void installTranslators(QCoreApplication *app)
{
    // Qt translator
    static QTranslator qtTr;
    qtTr.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app->installTranslator(&qtTr);

    // application translator
    static QTranslator myTr;
    myTr.load(QLocale::system().name(), langsPath());
    app->installTranslator(&myTr);
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    registerMetaTypes();

    installTranslators(&app);
    theme::init();
    plugin::soundprovider::Factory::init();

    WritableSound wsnd;
    wsnd.create(2, 100000);
    for (int i = 0; i < wsnd.sampleCount() * wsnd.channelCount(); ++i) {
        wsnd.samples()[i] = (500000 - qrand()%1000000) / 500000.0;
    }

    SoundWidget widget;
    widget.dataSource()->setSound(wsnd.toSound());
    widget.show();


    //MainWindow mainWindow;
    //mainWindow.show();

    return app.exec();
}
