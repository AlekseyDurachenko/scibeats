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
#ifndef CSOUNDPLOTWIDGET_H
#define CSOUNDPLOTWIDGET_H

#include <QWidget>
#include "qsecursorplot.h"
#include "qseselectionplot.h"
#include "qsesinglesignallinearplot.h"
#include "qsegeometry.h"
#include "qseabstractwidget.h"
#include "qsecoverplot.h"

class CSoundPlotWidget : public QseAbstractWidget
{
    Q_OBJECT
public:
    explicit CSoundPlotWidget(QWidget *parent = 0);

    inline QseSelectionPlot *selection();
    inline QseCursorPlot *play();
    inline QseCursorPlot *position();
    inline QseCoverPlot *cover();
    inline QseSingleSignalLinearPlot *plotter();
    void setSelection(QseSelectionPlot *selection);
    void setPlay(QseCursorPlot *cursor);
    void setPosition(QseCursorPlot *cursor);
    void setCover(QseCoverPlot *cover);
    void setPlotter(QseSingleSignalLinearPlot *plotter);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QImage m_waveformImage;
    QseSelectionPlot *m_selection;
    QseCursorPlot *m_play;
    QseCursorPlot *m_position;
    QseCoverPlot *m_coverPlot;
    QseSingleSignalLinearPlot *m_waveformPlot;

};

QseSelectionPlot *CSoundPlotWidget::selection()
{
    return m_selection;
}

QseCursorPlot *CSoundPlotWidget::play()
{
    return m_play;
}

QseCursorPlot *CSoundPlotWidget::position()
{
    return m_position;
}

QseCoverPlot *CSoundPlotWidget::cover()
{
    return m_coverPlot;
}

QseSingleSignalLinearPlot *CSoundPlotWidget::plotter()
{
    return m_waveformPlot;
}

#endif // CSOUNDPLOTWIDGET_H
