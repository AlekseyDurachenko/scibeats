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
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QNetworkAccessManager>
#include <QCloseEvent>
#include "csoundwidget.h"
#include "plugins/playbackend/plugin_playbackend_cabstractplaybackend.h"

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
    void on_action_Loop_triggered(bool checked);
    void on_action_PlayAll_triggered();
    void on_action_Play_triggered();
    void on_action_Pause_triggered();
    void on_action_Stop_triggered();
    void on_action_GoToStart_triggered();
    void on_action_GoToEnd_triggered();
    void on_action_Export_triggered();
    void on_action_ExportSelection_triggered();
    void on_action_Property_triggered();
    void on_action_Quit_triggered();
    void on_action_ZoomIn_triggered();
    void on_action_ZoomOut_triggered();
    void on_action_FitInWindow_triggered();
    void on_action_ZoomToSelection_triggered();
    void on_action_About_triggered();
    void on_action_AboutQt_triggered();

    void slot_soundCreated(const CSound &sound);
    void slot_playActionChanged();
    void slot_selectionChanged();
    void slot_speedSlider_valueChanged(int value);
    void slot_speedDoubleSpinBox_valueChanged(double value);
    void slot_volumeSpinBox_valueChanged(int value);

protected:
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void setWindowTitleForSound(const CSound &sound);
    void setPlayBackend(const QByteArray &id);

private:
    Ui::CMainWindow *ui;
    QToolBar *m_speedToolBar;
    QSlider *m_speedSlider;
    QDoubleSpinBox *m_speedDoubleSpinBox;
    QToolBar *m_volumeToolBar;
    QSlider *m_volumeSlider;
    QSpinBox *m_volumeSpinBox;
    QNetworkAccessManager *m_network;
    CSoundWidget *m_soundWidget;
    CSound m_sound;
    bool m_isPlayAll;
    plugin::playbackend::CAbstractPlayBackend *m_playBackend;
    int m_timerId;

};

#endif // CMAINWINDOW_H
