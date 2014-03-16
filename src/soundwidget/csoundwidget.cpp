// Copyright 2013, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "csoundwidget.h"
#include <QtGui>
#include <QVBoxLayout>
#include <QDebug>
#include "qsefunc.h"
#include "qsetimemetricprovider.h"
#include "qseabsolutemetricprovider.h"

CSoundWidget::CSoundWidget(QWidget *parent) :
    QWidget(parent)
{
    // scrollbar widget
    m_scrollBar = new CSoundScrollBarWidget(this);
    connect(m_scrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(slot_scrollBar_valueChanged(int)));

    // time widget
    m_timeController = new QseHorizontalController(this);
    m_timeProvider = new QseTimeMetricProvider(this);
    m_timeWidget = new QseAxisWidget(QseAxisWidget::Top, this);
    m_timeWidget->setController(m_timeController);
    m_timeWidget->setMetricProvider(m_timeProvider);
    connect(m_timeController, SIGNAL(geometryChanged(QseGeometry)),
            this, SLOT(setGeometry(QseGeometry)));

    // amplitude widget
    m_amplitudeController = new QseVerticalController(this);
    m_amplitudeProvider = new QseAbsoluteMetricProvider(this);
    m_amplitudeWidget = new QseAxisWidget(QseAxisWidget::Left, this);
    m_amplitudeWidget->setController(m_amplitudeController);
    m_amplitudeWidget->setMetricProvider(m_amplitudeProvider);
    connect(m_amplitudeController, SIGNAL(geometryChanged(QseGeometry)),
            this, SLOT(setGeometry(QseGeometry)));

    // selection
    m_selection = new QseSelection(this);
    m_selectionPlot = new QseSelectionPlot(this);
    m_selectionPlot->setSelection(m_selection);

    // play
    m_playCursor = new QseCursor(this);
    m_playCursorPlot = new QseCursorPlot(this);
    m_playCursorPlot->setPen(QPen(Qt::darkGreen));
    m_playCursorPlot->setCursor(m_playCursor);

    // position
    m_positionCursor = new QseCursor(this);
    m_positionCursorPlot = new QseCursorPlot(this);
    m_positionCursorPlot->setCursor(m_positionCursor);

    // sound controller
    m_soundController = new CSoundPlotController(this);
    m_soundController->setPosition(m_positionCursor);
    m_soundController->setSelection(m_selection);
    connect(m_soundController, SIGNAL(geometryChanged(QseGeometry)),
            this, SLOT(setGeometry(QseGeometry)));
    connect(m_soundController, SIGNAL(playClicked(qint64)),
            this, SLOT(slot_play(qint64)));

    // sound widget
    m_soundCoverPlot = new QseCoverPlot(this);
    m_soundCoverPlot->setHorizontalMetricProvider(m_timeProvider);
    m_soundCoverPlot->setVerticalMetricProvider(m_amplitudeProvider);
    m_soundLinearPlot = new QseSingleSignalLinearPlot(this);
    m_soundPlotWidget = new CSoundPlotWidget(this);
    m_soundPlotWidget->setCover(m_soundCoverPlot);
    m_soundPlotWidget->setPlotter(m_soundLinearPlot);
    m_soundPlotWidget->setSelection(m_selectionPlot);
    m_soundPlotWidget->setPlay(m_playCursorPlot);
    m_soundPlotWidget->setPosition(m_positionCursorPlot);
    m_soundPlotWidget->setController(m_soundController);
    m_soundPlotWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // compose the sound viewer
    QGridLayout *grid = new QGridLayout;
    grid->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->addWidget(m_timeWidget, 0, 1);
    grid->addWidget(m_amplitudeWidget, 1, 0);
    grid->addWidget(m_soundPlotWidget, 1, 1);
    grid->addWidget(m_scrollBar, 2, 1);
    setLayout(grid);

    // waveform will read in separate thread
    m_soundPeakReader = new CSoundPeakReader;
    m_soundLinearPlot->setPeakReader(m_soundPeakReader);
    m_soundPeakReaderThread = new QThread(this);
    m_soundPeakReaderThread->start();
    m_soundPeakReader->moveToThread(m_soundPeakReaderThread);
}

CSoundWidget::~CSoundWidget()
{
    m_soundPeakReader->abort();
    m_soundPeakReaderThread->quit();
    m_soundPeakReaderThread->wait();
}

void CSoundWidget::setSound(const CSound& sound)
{
    if (m_soundPeakReader->sound() != sound)
    {
        m_soundPeakReader->setSound(sound);

        m_selection->setMaximum(sound.frameCount() - 1);
        m_playCursor->setMaximum(sound.frameCount() - 1);
        m_positionCursor->setMaximum(sound.frameCount() - 1);
        m_timeProvider->setSampleRate(sound.sampleRate());

        m_selection->reset();
        m_playCursor->reset();
        m_positionCursor->reset();
        m_soundLinearPlot->reset();

        setGeometry(geometry());
    }
}

void CSoundWidget::setGeometry(const QseGeometry &geometry)
{
    if (m_soundPlotWidget->geometry() != geometry ||
            sound().frameCount() != m_scrollBar->maximum())
    {
        QseGeometry newGeometry = geometry;
        if (geometry.x() < 0)
            newGeometry.setX(0);
        if (geometry.y() < -1.0)
            newGeometry.setY(-1.0);
        if (geometry.y() > 1.0)
            newGeometry.setY(1.0);

        m_soundPlotWidget->setGeometry(newGeometry);
        m_amplitudeWidget->setGeometry(newGeometry);
        m_timeWidget->setGeometry(newGeometry);
        m_scrollBar->setSoundPosition(newGeometry.x(),
                QseFunc::width(newGeometry.samplePerPixel(),
                        sound().frameCount()));
    }
}

void CSoundWidget::slot_play(qint64 index)
{
    if (index >= 0 && index < sound().frameCount())
    {
        emit playNeeded(index);
    }
}

void CSoundWidget::slot_scrollBar_valueChanged(int value)
{
    setGeometry(QseGeometry(value, geometry().y(),
            geometry().samplePerPixel(), geometry().height()));
}
