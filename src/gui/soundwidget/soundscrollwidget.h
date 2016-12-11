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
#ifndef SOUNDSCROLLWIDGET_H
#define SOUNDSCROLLWIDGET_H


#include "sounddatasource.h"
#include "qsegeometry.h"
#include <QAction>
#include <QToolButton>
#include <QScrollBar>
#include <QWidget>


class SoundScrollWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundScrollWidget(QWidget *parent = 0);

    inline SoundDataSource *dataSource() const;
    void setDataSource(SoundDataSource *dataSource);

signals:
    void geometryChanged(const QseGeometry &geometry);

public slots:
    void setGeometry(const QseGeometry &geometry);
    void setWidgetWidth(int width);

private slots:
    void action_zoomInHorizontal_triggered();
    void action_zoomOutHorizontal_triggered();
    void action_zoomAllHorizontal_triggered();
    void action_zoomInVertical_triggered();
    void action_zoomOutVertical_triggered();
    void action_zoomAllVertical_triggered();
    void horizontalScrollBar_valueChanged(int value);
    void dataSource_dataChanged();
    void dataSource_destroyed();

private:
    void createActions();
    void createToolButtons();
    void createHorizontalScrollBar();
    void createLayout();
    void updateScrollBar();

private:
    QAction *m_zoomInHorizontalAction;
    QAction *m_zoomOutHorizontalAction;
    QAction *m_zoomAllHorizontalAction;

    QAction *m_zoomInVerticalAction;
    QAction *m_zoomOutVerticalAction;
    QAction *m_zoomAllVerticalAction;

    QToolButton *m_zoomInHorizontalToolButton;
    QToolButton *m_zoomOutHorizontalToolButton;
    QToolButton *m_zoomAllHorizontalToolButton;

    QToolButton *m_zoomInVerticalToolButton;
    QToolButton *m_zoomOutVerticalToolButton;
    QToolButton *m_zoomAllVerticalToolButton;

    QScrollBar *m_horizontalScrollBar;

    SoundDataSource *m_dataSource;
    QseGeometry m_lastGeometry;
    int m_widgetWidth;
};

SoundDataSource *SoundScrollWidget::dataSource() const
{
    return m_dataSource;
}


#endif // SOUNDSCROLLWIDGET_H

