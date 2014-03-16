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
#include "csoundplotcontroller.h"
#include "qsefunc.h"


CSoundPlotController::CSoundPlotController(QObject *parent) :
    QseAbstractController(parent)
{
    m_position = 0;
    m_selection = 0;
    m_dragAction = false;
}

void CSoundPlotController::setPosition(QseCursor *cursor)
{
    if (cursor != m_position)
        m_position = cursor;
}

void CSoundPlotController::setSelection(QseSelection *selection)
{
    if (selection != m_selection)
        m_selection = selection;
}

QCursor CSoundPlotController::defaultCursor() const
{
    return QCursor(Qt::IBeamCursor);
}

void CSoundPlotController::mouseMoveEvent(QMouseEvent *event, const QRect &rect, const QseGeometry &geometry)
{
    qint64 sample = QseFunc::mapWidgetToSample(event->x(), geometry.x(), geometry.samplePerPixel());
    // if left button is pressed, we shuld change the selection accrording to the current
    // cursor position
    if (m_dragAction)
    {
        m_selection->setSelection(m_otherDragSample, sample);

        // if selection setted, we should hide the position cursor
        // for escape the ambiguity of the start play position
        if (!m_selection->isNull() && !m_position->isNull())
            m_position->reset();
    }

    udpateCursor(event->x(), rect, geometry);
}

void CSoundPlotController::mousePressEvent(QMouseEvent *event, const QRect &rect, const QseGeometry &geometry)
{
    qint64 sample = QseFunc::mapWidgetToSample(event->x(), geometry.x(), geometry.samplePerPixel());

    // for some situation the function 'play-by-click' is very useful. we realize the same
    // function by left-button-click + ctrl-button-pressed
    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier)
    {
        emit playClicked(sample);
    }
    else if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier)
    {
        // when we have a selection, we should test the drag possible,
        // if cursor placed around of the border of the selection
        // we start the change the selection
        if (!m_selection->isNull() && geometry.samplePerPixel() > 0)
        {
            qint64 diff_left = qAbs(event->x() + geometry.x() - QseFunc::width(geometry.samplePerPixel(), m_selection->left()));
            qint64 diff_right = qAbs(event->x() + geometry.x() - QseFunc::width(geometry.samplePerPixel(), m_selection->right()));

            if (diff_left < 5)
            {
                m_dragAction = 1;
                m_otherDragSample = m_selection->right();
            }
            if (diff_right < 5)
            {
                m_dragAction = 1;
                m_otherDragSample = m_selection->left();
            }
        }
        else if (!m_selection->isNull())
        {
            qint64 diff_left = qAbs(geometry.x() + event->x() / qAbs(geometry.samplePerPixel()) - m_selection->left());
            qint64 diff_right = qAbs(geometry.x() + event->x() / qAbs(geometry.samplePerPixel()) - m_selection->right());
            qint64 px_left = diff_left * qAbs(geometry.samplePerPixel());
            qint64 px_right = diff_right * qAbs(geometry.samplePerPixel());

            if (px_left < 5)
            {
                m_dragAction = 1;
                m_otherDragSample = m_selection->right();
            }
            if (px_right < 5)
            {
                m_dragAction = 1;
                m_otherDragSample = m_selection->left();
            }
        }

        // if we click out of selection bound we should set
        // cursor, and reset selection if it exists
        if (m_dragAction == false)
        {
            if (!m_selection->isNull())
                m_selection->reset();

            m_position->setIndex(sample);

            m_dragAction = true;
            m_otherDragSample = sample;
            m_selection->setSelection(sample, sample);
        }
    }
    // shen we keep the shif with mouse left button click, we should
    // edit the selection
    else if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ShiftModifier)
    {
        // when selection is exists, we extend it to the current cursor position
        if (!m_selection->isNull())
        {
            if (sample > m_selection->right())
                m_selection->setSelection(m_selection->left(), sample);
            else if (sample < m_selection->left())
                m_selection->setSelection(sample, m_selection->right());
            // when we in the selection range, we set left() of right()
            // bound of the selection, wich more closely to the cursor
            else if (m_selection->right() - sample < sample - m_selection->left())
                m_selection->setSelection(m_selection->left(), sample);
            else
                m_selection->setSelection(sample, m_selection->right());
        }
        // when position cursor is set, we create selection from this position
        // to the current cursor position
        else if (!m_position->isNull())
        {
            if (sample > m_position->index())
                m_selection->setSelection(m_position->index(), sample);
            else if (sample < m_position->index())
                m_selection->setSelection(sample, m_position->index());
        }

        // if selection was setted, position cursor must be cleared
        if (!m_selection->isNull())
            m_position->reset();
    }

    udpateCursor(event->x(), rect, geometry);
}

void CSoundPlotController::mouseReleaseEvent(QMouseEvent *event, const QRect &rect, const QseGeometry &geometry)
{
    // after left button of the mouse will be release
    // we should finished the selection
    if (!(event->buttons() & Qt::LeftButton))
        m_dragAction = false;

    udpateCursor(event->x(), rect, geometry);
}

void CSoundPlotController::wheelEvent(QWheelEvent *event, const QRect &rect, const QseGeometry &geometry)
{
    // mouse scroll width SHIFT pressed:
    // scroll the waveform (scroll down - next samples, scroll up - prev samples)
    if (event->modifiers() == Qt::ShiftModifier)
    {
        emit geometryChanged(geometry.addX(-event->delta()/4));
    }
    // mouse scroll width CTRL pressed:
    // change of the visible height (scroll down - expand height, scroll up - collalps height)
    else if (event->modifiers() == Qt::ControlModifier)
    {
        QseGeometry newGeometry = geometry;
        if (event->delta() > 0)
            newGeometry.setHeight(geometry.height()/qAbs(event->delta()/120)/1.5);
        else if (event->delta() < 0)
            newGeometry.setHeight(geometry.height()*qAbs(event->delta()/120)*1.5);
        emit geometryChanged(newGeometry);
    }
    // mouse scroll width NO BUTTON pressed:
    // change the scale factor of the waveform (scroll up - zoom in, scroll down - zoom out)
    else if (event->modifiers() == Qt::NoModifier)
    {
        // calculate zoom
        int degree = event->delta() / 60;
        qint64 spp = geometry.samplePerPixel();
        // zoom in
        if (degree > 0)
        {
            if (spp > 0)
                spp /= qAbs(degree);
            else
                spp *= qAbs(degree);
            if (spp == 0 || spp == 1)
                spp = -1;
        }
        // zoom out
        else if (degree < 0)
        {
            if (spp > 0)
                spp *= qAbs(degree);
            else
                spp /= qAbs(degree);
            if (spp == 0 || spp == 1)
                spp = 2;
        }

        // if we change the zoom, we sould chang the current offset, for sample under the cursor
        // will be equal to the sample under the cursor after zooming
        qint64 sample = QseFunc::mapWidgetToSample(event->x(), geometry.x(), geometry.samplePerPixel());
        qint64 offset = 0;
        if (spp > 0)
            offset = sample / spp + ((sample % spp) ? 1 : 0) - event->x();
        if (spp < 0)
            offset = sample - qRound(static_cast<qreal>(event->x()) / qAbs(spp));

        emit geometryChanged(QseGeometry(offset, geometry.y(), spp, geometry.height()));
    }

    udpateCursor(event->x(), rect, geometry);
}

void CSoundPlotController::udpateCursor(int x, const QRect &, const QseGeometry &geometry)
{
    // we click on the some position, set position cursor and ready to the
    // create selection range
    if (m_dragAction && m_selection->isNull())
    {
        emit cursorChanged(QCursor(Qt::IBeamCursor));
    }
    // we moved the selection range
    else if (m_dragAction)
    {
        emit cursorChanged(QCursor(Qt::SizeHorCursor));
    }
    // default cursor
    else if (m_selection->isNull())
    {
        emit cursorChanged(defaultCursor());
    }
    // cursor closely the selection round
    else if (geometry.samplePerPixel() > 0)
    {
        qint64 diff_left = qAbs(x + geometry.x() - QseFunc::width(geometry.samplePerPixel(), m_selection->left()));
        qint64 diff_right = qAbs(x + geometry.x() - QseFunc::width(geometry.samplePerPixel(), m_selection->right()));

        if (diff_left < 5 || diff_right < 5)
            emit cursorChanged(QCursor(Qt::SizeHorCursor));
        else
            emit cursorChanged(QCursor(Qt::IBeamCursor));
    }
    else
    {
        qint64 diff_left = qAbs(geometry.x() + x / qAbs(geometry.samplePerPixel()) - m_selection->left());
        qint64 diff_right = qAbs(geometry.x() + x / qAbs(geometry.samplePerPixel()) - m_selection->right());
        qint64 px_left = diff_left * qAbs(geometry.samplePerPixel());
        qint64 px_right = diff_right * qAbs(geometry.samplePerPixel());

        if (px_left < 5 || px_right < 5)
            emit cursorChanged(QCursor(Qt::SizeHorCursor));
        else
            emit cursorChanged(QCursor(Qt::IBeamCursor));
    }
}
