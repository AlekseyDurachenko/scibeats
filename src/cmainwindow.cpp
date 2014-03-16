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
#include "cmainwindow.h"
#include "csoundwidget.h"
#include "ui_cmainwindow.h"
#include "plugins/playbackend/plugin_playbackend_factory.h"
#include "cexportdialog.h"
#include <QNetworkAccessManager>
#include <QDebug>
#include <QtGui>
#include "cdatasourcewidget.h"
#include "csoundpropertywidget.h"
#include "global.h"
#include "caboutdialog.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::CMainWindow)
{
    ui->setupUi(this);

    // setup sound widget
    m_soundWidget = new CSoundWidget(this);
    m_soundWidget->setEnabled(false);
    connect(m_soundWidget->selection(), SIGNAL(changed()),
            this, SLOT(slot_selectionChanged()));
    setCentralWidget(m_soundWidget);

    // setup datasource
    m_network = new QNetworkAccessManager(this);
    CDataSourceWidget *widget = new CDataSourceWidget(m_network, this);
    connect(widget, SIGNAL(soundCreated(CSound)),
            this, SLOT(slot_soundCreated(CSound)));
    ui->dockWidget->setWidget(widget);

    // setup the speed controller
    m_speedToolBar = new QToolBar(tr("Speed controll"));
    m_speedSlider = new QSlider(Qt::Horizontal, this);
    m_speedSlider->setRange(0, 1090);
    m_speedSlider->setValue(100);
    m_speedDoubleSpinBox = new QDoubleSpinBox(this);
    m_speedDoubleSpinBox->setRange(0.01, 100.0);
    m_speedDoubleSpinBox->setValue(1.0);
    m_speedDoubleSpinBox->setSuffix("x");
    QWidget *speedWidget = new QWidget(this);
    speedWidget->setLayout(new QHBoxLayout);
    speedWidget->layout()->setContentsMargins(8, 4, 4, 8);
    speedWidget->layout()->setSpacing(5);
    speedWidget->layout()->addWidget(new QLabel(tr("Speed"), this));
    speedWidget->layout()->addWidget(m_speedSlider);
    speedWidget->layout()->addWidget(m_speedDoubleSpinBox);
    m_speedToolBar->addWidget(speedWidget);
    connect(m_speedSlider, SIGNAL(valueChanged(int)),
            this, SLOT(slot_speedSlider_valueChanged(int)));
    connect(m_speedDoubleSpinBox, SIGNAL(valueChanged(double)),
            this, SLOT(slot_speedDoubleSpinBox_valueChanged(double)));
    addToolBar(m_speedToolBar);

    // setup the volume controller
    m_volumeToolBar = new QToolBar(tr("Volume controll"));
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(100);
    m_volumeSpinBox = new QSpinBox(this);
    m_volumeSpinBox->setRange(0, 100);
    m_volumeSpinBox->setValue(100);
    m_volumeSpinBox->setSuffix("%");
    QWidget *volumeWidget = new QWidget(this);
    volumeWidget->setLayout(new QHBoxLayout);
    volumeWidget->layout()->setContentsMargins(8, 4, 4, 8);
    volumeWidget->layout()->setSpacing(5);
    volumeWidget->layout()->addWidget(new QLabel(tr("Volume"), this));
    volumeWidget->layout()->addWidget(m_volumeSlider);
    volumeWidget->layout()->addWidget(m_volumeSpinBox);
    m_volumeToolBar->addWidget(volumeWidget);
    connect(m_volumeSlider, SIGNAL(valueChanged(int)),
            m_volumeSpinBox, SLOT(setValue(int)));
    connect(m_volumeSpinBox, SIGNAL(valueChanged(int)),
            m_volumeSlider, SLOT(setValue(int)));
    connect(m_volumeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(slot_volumeSpinBox_valueChanged(int)));
    addToolBar(m_volumeToolBar);

    //
    m_isPlayAll = false;
    m_playBackend = 0;
    m_timerId = startTimer(50);

    //
    setPlayBackend(plugin::playbackend::Factory::defaultId());
    slot_soundCreated(CSound());
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::on_action_Loop_triggered(bool checked)
{
    m_playBackend->setLoop(checked);
}

void CMainWindow::on_action_PlayAll_triggered()
{
    m_playBackend->setRange(0, m_soundWidget->sound().frameCount()-1);
    m_playBackend->setIndex(0);
    m_playBackend->play();
    m_isPlayAll = true;
}

void CMainWindow::on_action_Play_triggered()
{
    if (!m_soundWidget->selection()->isNull())
    {
        m_playBackend->setRange(m_soundWidget->selection()->left(),
                m_soundWidget->selection()->right());
        m_playBackend->setIndex(m_soundWidget->selection()->left());
    }
    else
    {
        m_playBackend->setRange(0, m_soundWidget->sound().frameCount()-1);
        if (!m_soundWidget->positionCursor()->isNull())
        {
            m_playBackend->setIndex(m_soundWidget->positionCursor()->index());
        }
        else
        {
            m_playBackend->setIndex(0);
        }
    }
    m_playBackend->play();
    m_isPlayAll = false;
}

void CMainWindow::on_action_Pause_triggered()
{
    m_playBackend->pause();
}

void CMainWindow::on_action_Stop_triggered()
{
    m_playBackend->stop();
}

void CMainWindow::on_action_GoToStart_triggered()
{
    if (m_playBackend->isStoped())
    {
        m_soundWidget->positionCursor()->setIndex(0);
        m_playBackend->setIndex(0);
    }
    else if (m_soundWidget->selection()->isNull())
    {
        m_soundWidget->positionCursor()->setIndex(0);
        m_playBackend->setIndex(0);
    }
    else if (m_isPlayAll)
    {
        m_soundWidget->positionCursor()->setIndex(0);
        m_playBackend->setIndex(0);
    }
    else
    {
        m_soundWidget->positionCursor()->setIndex(
                    m_soundWidget->selection()->left());
        m_playBackend->setIndex(m_soundWidget->selection()->left());
    }
}

void CMainWindow::on_action_GoToEnd_triggered()
{
    if (m_playBackend->isStoped())
    {
        m_soundWidget->positionCursor()->setIndex(0);
        m_playBackend->setIndex(0);
    }
    else if (m_soundWidget->selection()->isNull())
    {
        m_soundWidget->positionCursor()->setIndex(0);
        m_playBackend->setIndex(0);
    }
    else if (m_isPlayAll)
    {
        m_soundWidget->positionCursor()->setIndex(0);
        m_playBackend->setIndex(0);
    }
    else
    {
        m_soundWidget->positionCursor()->setIndex(
                    m_soundWidget->selection()->right());
        m_playBackend->setIndex(m_soundWidget->selection()->right());
    }
}

void CMainWindow::on_action_Export_triggered()
{
    CExportDialog exportDialog(this);
    exportDialog.setWindowTitle(tr("Export"));
    exportDialog.setSound(m_sound);
    exportDialog.setPlaySampleRate(m_playBackend->sampleRate());
    exportDialog.setRange(0, m_sound.frameCount()-1);
    exportDialog.setSpeed(m_speedDoubleSpinBox->value());
    exportDialog.exec();
}

void CMainWindow::on_action_ExportSelection_triggered()
{
    CExportDialog exportDialog(this);
    exportDialog.setWindowTitle(tr("Export selection"));
    exportDialog.setSound(m_sound);
    exportDialog.setPlaySampleRate(m_playBackend->sampleRate());
    exportDialog.setRange(m_soundWidget->selection()->left(),
            m_soundWidget->selection()->right());
    exportDialog.setSpeed(m_speedDoubleSpinBox->value());
    exportDialog.exec();
}

void CMainWindow::on_action_Property_triggered()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Sound property"));
    CSoundPropertyWidget propertyWidget(&dialog);
    propertyWidget.fromSound(m_sound);
    QDialogButtonBox buttonBox(&dialog);
    buttonBox.setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    QVBoxLayout layout;
    layout.addWidget(&propertyWidget);
    layout.addWidget(&buttonBox);
    dialog.setLayout(&layout);

    if (dialog.exec() == QDialog::Accepted)
    {
        propertyWidget.toSound(m_sound);
    }
}

void CMainWindow::on_action_Quit_triggered()
{
    close();
}

void CMainWindow::on_action_ZoomIn_triggered()
{
    QseGeometry geometry = m_soundWidget->geometry();
    geometry.setSamplePerPixel(geometry.samplePerPixel() / 2);
    m_soundWidget->setGeometry(geometry);
}

void CMainWindow::on_action_ZoomOut_triggered()
{
    QseGeometry geometry = m_soundWidget->geometry();
    geometry.setSamplePerPixel(geometry.samplePerPixel() * 2);
    m_soundWidget->setGeometry(geometry);
}

void CMainWindow::on_action_FitInWindow_triggered()
{
    if (!m_sound.isNull())
    {
        QseGeometry geometry;
        geometry.setX(0);
        geometry.setY(0.0);
        geometry.setHeight(2.0);
        if (m_soundWidget->width() < m_sound.frameCount())
        {
            geometry.setSamplePerPixel(
                    m_sound.frameCount() / m_soundWidget->width());
        }
        else
        {
            geometry.setSamplePerPixel(
                    -m_soundWidget->width() / m_sound.frameCount());
        }
        m_soundWidget->setGeometry(geometry);
    }
}

void CMainWindow::on_action_ZoomToSelection_triggered()
{
    QseGeometry geometry;
    geometry.setY(0.0);
    geometry.setHeight(2.0);
    if (m_soundWidget->width() < m_soundWidget->selection()->count())
    {
        geometry.setSamplePerPixel(
                m_soundWidget->selection()->count() / m_soundWidget->width());
        geometry.setX(
                m_soundWidget->selection()->left() / geometry.samplePerPixel());
    }
    else
    {
        geometry.setX(m_soundWidget->selection()->left());
        geometry.setSamplePerPixel(
                -m_soundWidget->width() / m_soundWidget->selection()->count());
    }
    m_soundWidget->setGeometry(geometry);
}

void CMainWindow::on_action_About_triggered()
{
    CAboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void CMainWindow::on_action_AboutQt_triggered()
{
    qApp->aboutQt();
}

void CMainWindow::slot_soundCreated(const CSound &sound)
{
    m_sound = sound;
    ui->action_Property->setDisabled(m_sound.isNull());
    ui->action_Export->setDisabled(m_sound.isNull());
    ui->action_ZoomIn->setDisabled(m_sound.isNull());
    ui->action_ZoomOut->setDisabled(m_sound.isNull());
    ui->action_FitInWindow->setDisabled(m_sound.isNull());
    m_soundWidget->setDisabled(m_sound.isNull());
    m_soundWidget->setSound(m_sound);
    m_playBackend->setSound(m_sound);
    setWindowTitleForSound(m_sound);
    slot_playActionChanged();
    slot_selectionChanged();
    on_action_FitInWindow_triggered();
}

void CMainWindow::slot_playActionChanged()
{
    if (m_sound.isNull())
    {
        ui->action_PlayAll->setEnabled(false);
        ui->action_Play->setEnabled(false);
        ui->action_Pause->setEnabled(false);
        ui->action_Stop->setEnabled(false);
        ui->action_GoToStart->setEnabled(false);
        ui->action_GoToEnd->setEnabled(false);
    }
    else if (m_playBackend->isStoped())
    {
        ui->action_PlayAll->setEnabled(true);
        ui->action_Play->setEnabled(true);
        ui->action_Pause->setEnabled(false);
        ui->action_Stop->setEnabled(false);
        ui->action_GoToStart->setEnabled(true);
        ui->action_GoToEnd->setEnabled(true);
        m_soundWidget->playCursor()->reset();
    }
    else if (m_playBackend->isPaused())
    {
        ui->action_PlayAll->setEnabled(true);
        ui->action_Play->setEnabled(true);
        ui->action_Pause->setEnabled(true);
        ui->action_Stop->setEnabled(true);
        ui->action_GoToStart->setEnabled(true);
        ui->action_GoToEnd->setEnabled(true);
    }
    else if (m_playBackend->isPlayed())
    {
        ui->action_PlayAll->setEnabled(true);
        ui->action_Play->setEnabled(true);
        ui->action_Pause->setEnabled(true);
        ui->action_Stop->setEnabled(true);
        ui->action_GoToStart->setEnabled(true);
        ui->action_GoToEnd->setEnabled(true);
    }
}

void CMainWindow::slot_selectionChanged()
{
    if (!m_isPlayAll)
    {
        if (!m_soundWidget->selection()->isNull())
        {
            m_playBackend->setRange(m_soundWidget->selection()->left(),
                    m_soundWidget->selection()->right());
        }
        else
        {
            m_playBackend->setRange(0, m_soundWidget->sound().frameCount()-1);
        }
    }

    ui->action_ExportSelection->setDisabled(
            m_soundWidget->selection()->isNull());
    ui->action_ZoomToSelection->setDisabled(
            m_soundWidget->selection()->isNull());
}

void CMainWindow::slot_speedSlider_valueChanged(int value)
{
    if (value < 100)
    {
        m_speedDoubleSpinBox->setValue(value/100.0);
    }
    else
    {
        m_speedDoubleSpinBox->setValue(1.0 + (value-100)/10.0);
    }
}

void CMainWindow::slot_speedDoubleSpinBox_valueChanged(double value)
{
    if (value < 1.0)
    {
        m_speedSlider->setValue(value*100);
    }
    else
    {
        m_speedSlider->setValue(100+(value-1.0)*10);
    }
    m_playBackend->setSpeed(value);
}

void CMainWindow::slot_volumeSpinBox_valueChanged(int value)
{
    m_playBackend->setVolume(value/100.0);
}

void CMainWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId)
    {
        if (m_playBackend && m_playBackend->isPlayed())
        {
            m_soundWidget->playCursor()->setIndex(m_playBackend->index());
        }
    }
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    if (m_sound.isNull())
    {
        event->accept();
    }
    else
    {
        int ret = QMessageBox::question(this, tr("Quit"),
                tr("Are you sure you want to quit?"),
                QMessageBox::Yes|QMessageBox::No);

        if (ret == QMessageBox::Yes)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void CMainWindow::setWindowTitleForSound(const CSound &sound)
{
    if (sound.isNull())
    {
        setWindowTitle(G_FULL_PROGRAM_NAME());
    }
    else
    {
        setWindowTitle(QString("%1 \"%2\" - %3")
                .arg(sound.property(CSound::Artist))
                .arg(sound.property(CSound::Title))
                .arg(G_FULL_PROGRAM_NAME()));
    }
}

void CMainWindow::setPlayBackend(const QByteArray &id)
{
    m_playBackend = plugin::playbackend::Factory::createBackend(id, this);
    connect(m_playBackend, SIGNAL(indexChanged(qint64)),
            m_soundWidget->playCursor(), SLOT(setIndex(qint64)));
    connect(m_playBackend, SIGNAL(played()),
            this, SLOT(slot_playActionChanged()));
    connect(m_playBackend, SIGNAL(paused()),
            this, SLOT(slot_playActionChanged()));
    connect(m_playBackend, SIGNAL(stoped()),
            this, SLOT(slot_playActionChanged()));
    m_playBackend->setLoop(ui->action_Loop->isChecked());
}
