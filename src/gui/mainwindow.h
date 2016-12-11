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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "soundpeakscalculator.h"
#include "soundwriter.h"
#include "soundprovider.h"
#include <QMainWindow>
#include <QNetworkAccessManager>


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public:
    QNetworkAccessManager *network() const;

    SoundWriter *soundWriter() const;
    SoundProvider *soundProvider() const;
    SoundPeaksCalculator *soundPeaksCalculator() const;

private:
    bool isRequestSupported(const SoundProviderRequest &request, QString *reason = 0) const;
    void createSoundWindow(const SoundProviderRequest &request);

private:
    void createNetwork();
    void createSoundWriter();
    void createSoundProvider();
    void createSoundPeakCalculator();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *m_network;
    SoundWriter *m_soundWriter;
    SoundProvider *m_soundProvider;
    SoundPeaksCalculator *m_soundPeaksCalculator;
};


#endif // MAINWINDOW_H
