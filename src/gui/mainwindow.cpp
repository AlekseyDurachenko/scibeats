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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "soundwidget.h"
#include "soundwindow.h"
#include "theme.h"
#include "plugin_soundprovider_factory.h"


static QByteArray fromRc(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(theme::icon(theme::Icon::App));

    createNetwork();

    createSoundWriter();
    createSoundProvider();
    createSoundPeakCalculator();

    SoundProviderRequest request;
    //request.setQuery(fromRc(":/tests/test_plugin_soundprovider_factory_findid_test_1.xml"));
    request.setQuery(fromRc(":/tests/test_plugin_soundprovider_fdsnws_parameters_test_1.xml"));
    createSoundWindow(request);
    createSoundWindow(request);
    createSoundWindow(request);
}

MainWindow::~MainWindow()
{
    delete ui;
}

SoundWriter *MainWindow::soundWriter() const
{
    return m_soundWriter;
}

SoundProvider *MainWindow::soundProvider() const
{
    return m_soundProvider;
}

SoundPeaksCalculator *MainWindow::soundPeaksCalculator() const
{
    return m_soundPeaksCalculator;
}

QNetworkAccessManager *MainWindow::network() const
{
    return m_network;
}

bool MainWindow::isRequestSupported(const SoundProviderRequest &request, QString *reason) const
{
    return plugin::soundprovider::Factory::validateRequest(request, reason);
}

void MainWindow::createSoundWindow(const SoundProviderRequest &request)
{
    SoundWindow *soundWindow = new SoundWindow();
    soundWindow->setSoundWriter(soundWriter());
    soundWindow->setSoundProvider(soundProvider());
    soundWindow->setSoundPeaksCalculator(soundPeaksCalculator());
    soundWindow->setRequest(request);
    soundWindow->openSound();
    ui->mdiArea->addSubWindow(soundWindow);
}

void MainWindow::createNetwork()
{
    m_network = new QNetworkAccessManager(this);
}

void MainWindow::createSoundWriter()
{
    m_soundWriter = new SoundWriter(this);
}

void MainWindow::createSoundProvider()
{
    m_soundProvider = new SoundProvider(network(), this);
}

void MainWindow::createSoundPeakCalculator()
{
    m_soundPeaksCalculator = new SoundPeaksCalculator(this);
}
