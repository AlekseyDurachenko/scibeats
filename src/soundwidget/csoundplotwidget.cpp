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
#include "csoundplotwidget.h"

CSoundPlotWidget::CSoundPlotWidget(QWidget *parent) :
    QseAbstractWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);

    m_selection = 0;
    m_play = 0;
    m_position = 0;
    m_waveformPlot = 0;
    m_coverPlot = 0;
}

void CSoundPlotWidget::setSelection(QseSelectionPlot *selection)
{
    if (selection != m_selection)
    {
        if (m_selection)
        {
            disconnect(m_selection, 0, this, 0);
        }
        if (selection)
        {
            connect(selection, SIGNAL(changed()), this, SLOT(update()));
        }
        m_selection = selection;

        update();
    }
}

void CSoundPlotWidget::setPlay(QseCursorPlot *cursor)
{
    if (cursor != m_play)
    {
        if (m_play)
        {
            disconnect(m_play, 0, this, 0);
        }
        if (cursor)
        {
            connect(cursor, SIGNAL(changed()), this, SLOT(update()));
        }
        m_play = cursor;

        update();
    }
}

void CSoundPlotWidget::setPosition(QseCursorPlot *cursor)
{
    if (cursor != m_position)
    {
        if (m_position)
        {
            disconnect(m_position, 0, this, 0);
        }
        if (cursor)
        {
            connect(cursor, SIGNAL(changed()), this, SLOT(update()));
        }
        m_position = cursor;

        update();
    }
}

void CSoundPlotWidget::setCover(QseCoverPlot *cover)
{
    if (cover != m_coverPlot)
    {
        if (m_coverPlot)
        {
            disconnect(m_coverPlot, 0, this, 0);
        }
        if (cover)
        {
            connect(cover, SIGNAL(changed()), this, SLOT(update()));
        }
        m_coverPlot = cover;

        update();
    }
}

void CSoundPlotWidget::setPlotter(QseSingleSignalLinearPlot *plotter)
{
    if (plotter != m_waveformPlot)
    {
        if (m_waveformPlot)
        {
            disconnect(m_waveformPlot, 0, this, 0);
        }
        if (plotter)
        {
            connect(plotter, SIGNAL(changed()), this, SLOT(update()));
        }
        m_waveformPlot = plotter;

        update();
    }
}

void CSoundPlotWidget::paintEvent(QPaintEvent *)
{
    if (m_waveformPlot && m_waveformPlot->hasChanges(rect(), geometry()))
    {
        if (m_waveformImage.size() != rect().size())
        {
            m_waveformImage = QImage(rect().size(), QImage::Format_RGB32);
        }

        QPainter imagePainter(&m_waveformImage);
        // note: возможно, можно улучшить производительность
        // отрисовки, если рендерить m_coverPlot в отдельное
        // изображение. Но сперва необходимо протестировать
        // такой подход.
        if (m_coverPlot)
        {
            m_coverPlot->draw(&imagePainter, rect(), geometry());
        }
        m_waveformPlot->draw(&imagePainter, rect(), geometry());
    }

    QPainter painter(this);
    if (m_waveformPlot)
    {
        painter.drawImage(rect(), m_waveformImage);
    }
    if (m_selection)
    {
        m_selection->draw(&painter, rect(), geometry());
    }
    if (m_play)
    {
        m_play->draw(&painter, rect(), geometry());
    }
    if (m_position)
    {
        m_position->draw(&painter, rect(), geometry());
    }
}
