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
#ifndef SOUNDWIDGET_H
#define SOUNDWIDGET_H


#include "soundwriter.h"
#include "soundpeakscalculator.h"
#include "soundprovider.h"
#include "soundpeakscalculator.h"
#include "soundcontrolpanelwidget.h"
#include "soundscrollwidget.h"
#include "qseposition.h"
#include "qseselection.h"
#include "sounddatasource.h"
#include "qsewidget.h"
#include "qsehorizontalaxiswidget.h"
#include "qseverticalaxiswidget.h"
#include <QWidget>


namespace Ui {
class SoundWidget;
}

class SoundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundWidget(QWidget *parent = 0);

    inline QsePosition *playPosition() const;
    inline QsePosition *cursorPosition() const;
    inline QseSelection *selection() const;
    inline SoundDataSource *dataSource() const;

private:
    void createPlayPosition();
    void createCursorPosition();
    void createSelection();
    void createDataSource();
    void createControlPanelWidget();
    void createScrollWidget();
    void createLayout();

    QseWidget *createSignalWidget() const;
    QseHorizontalAxisWidget *createTimeWidget();
    QseVerticalAxisWidget *createAmplWidget();

private:
    Ui::SoundWidget *ui;

    QsePosition *m_playPosition;
    QsePosition *m_cursorPosition;
    QseSelection *m_selection;
    SoundDataSource *m_dataSource;

    SoundControlPanelWidget *m_controlPanelWidget;
    SoundScrollWidget *m_scrollWidget;
};


QsePosition *SoundWidget::playPosition() const
{
    return m_playPosition;
}

QsePosition *SoundWidget::cursorPosition() const
{
    return m_cursorPosition;
}

QseSelection *SoundWidget::selection() const
{
    return m_selection;
}

SoundDataSource *SoundWidget::dataSource() const
{
    return m_dataSource;
}


#endif // SOUNDWIDGET_H
