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
#ifndef CSOUNDPLOTCONTROLLER_H
#define CSOUNDPLOTCONTROLLER_H

#include "qseabstractcontroller.h"
#include "qseselection.h"
#include "qsecursor.h"

// The key combination:
// ctrl+LeftButtonClick - emit ctrlLeftClick()
// ctrl+wheel - height zoom
// wheel + shift - scroll the waveform
// wheel - width zoom with
// LeftButton+Move - selection
// shift + LeftButtonClick - expand selecton, if not exists then expand from cursor
// LeftButtonClick - set cursor
// if selection is exists cursor not exists, and otherwise
class CSoundPlotController : public QseAbstractController
{
    Q_OBJECT
public:
    explicit CSoundPlotController(QObject *parent = 0);

    // do not forget install position cursor, and selection
    // otherwise nothing will happen
    inline QseCursor *position();
    inline QseSelection *selection();
    void setPosition(QseCursor *cursor);
    void setSelection(QseSelection *selection);

    QCursor defaultCursor() const;
    void mouseMoveEvent(QMouseEvent *event,
            const QRect &rect, const QseGeometry &geometry);
    void mousePressEvent(QMouseEvent *event,
            const QRect &rect, const QseGeometry &geometry);
    void mouseReleaseEvent(QMouseEvent *event,
            const QRect &rect, const QseGeometry &geometry);
    void wheelEvent(QWheelEvent *event,
            const QRect &rect, const QseGeometry &geometry);
signals:
    // if user click on the widget with the controll modifier,
    // this signal will be emited
    void playClicked(qint64 index);
private:
    // when we press mouse or move mouse, we want see the some
    // cursor, for help determinate the current action.
    void udpateCursor(int x, const QRect &rect, const QseGeometry &geometry);
private:
    QseCursor *m_position;
    QseSelection *m_selection;
    // true  -- ready to move the selection
    // false -- not ready to move the selection
    bool m_dragAction;
    // store the one selection bound different the current cursor
    // selection round
    qint64 m_otherDragSample;
};


QseCursor *CSoundPlotController::position()
{
    return m_position;
}

QseSelection *CSoundPlotController::selection()
{
    return m_selection;
}


#endif // CSOUNDPLOTCONTROLLER_H
