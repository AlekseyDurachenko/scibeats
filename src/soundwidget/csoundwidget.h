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
#ifndef CSOUNDWIDGET_H
#define CSOUNDWIDGET_H

#include <QWidget>
#include <QThread>
#include <QSharedPointer>
#include "csoundscrollbarwidget.h"
#include "csoundpeakreader.h"
#include "csound.h"
#include "csoundplotwidget.h"
#include "qsesinglesignallinearplot.h"
#include "qsegeometry.h"
#include "qseaxiswidget.h"
#include "qsehorizontalcontroller.h"
#include "qseverticalcontroller.h"
#include "csoundplotcontroller.h"
#include "qsetimemetricprovider.h"
#include "qsecursor.h"
#include "qseselection.h"

// This widget represent the sound with single audio channel
// the selection, play cursor, position cursor are supported.
// Note: the sound mustn't changed after setSound()
class CSoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSoundWidget(QWidget *parent = 0);
    ~CSoundWidget();

    inline const QseGeometry &geometry() const;
    inline const CSound &sound() const;

    inline QseSelection *selection();
    inline QseCursor *playCursor();
    inline QseCursor *positionCursor();

signals:
    void playNeeded(qint64 index);

public slots:
    void setSound(const CSound& sound);
    void setGeometry(const QseGeometry &geometry);

private slots:
    void slot_play(qint64 index);
    void slot_scrollBar_valueChanged(int value);

private:
    // scrollbar widget
    CSoundScrollBarWidget *m_scrollBar;
    // time widget
    QseAxisWidget *m_timeWidget;
    QseTimeMetricProvider *m_timeProvider;
    QseHorizontalController *m_timeController;
    // amplitude widget
    QseAxisWidget *m_amplitudeWidget;
    QseAbsoluteMetricProvider *m_amplitudeProvider;
    QseVerticalController *m_amplitudeController;
    // sound widget
    CSoundPlotWidget *m_soundPlotWidget;
    QseCoverPlot *m_soundCoverPlot;
    QseSingleSignalLinearPlot *m_soundLinearPlot;
    CSoundPlotController *m_soundController;
    CSoundPeakReader *m_soundPeakReader;
    QThread *m_soundPeakReaderThread;
    QseSelection *m_selection;
    QseSelectionPlot *m_selectionPlot;
    QseCursor *m_playCursor;
    QseCursorPlot *m_playCursorPlot;
    QseCursor *m_positionCursor;
    QseCursorPlot *m_positionCursorPlot;

};

const QseGeometry &CSoundWidget::geometry() const
{
    return m_soundPlotWidget->geometry();
}

const CSound &CSoundWidget::sound() const
{
    return m_soundPeakReader->sound();
}

QseSelection *CSoundWidget::selection()
{
    return m_selection;
}

QseCursor *CSoundWidget::positionCursor()
{
    return m_positionCursor;
}

QseCursor *CSoundWidget::playCursor()
{
    return m_playCursor;
}

#endif // CVIEWERWIDGET_H
