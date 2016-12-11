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
#ifndef SOUNDCONTROLPANELWIDGET_H
#define SOUNDCONTROLPANELWIDGET_H


#include "sounddatasource.h"
#include "soundcontrolbuttonboxwidget.h"
#include "soundcontrolsampleratewidget.h"
#include "soundcontrolselectionwidget.h"
#include "soundcontrolgeometrywidget.h"
#include "qsegeometry.h"
#include "qseselection.h"
#include "qseposition.h"
#include <QWidget>


class SoundControlPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundControlPanelWidget(QWidget *parent = 0);

    inline SoundDataSource *dataSource() const;
    void setDataSource(SoundDataSource *dataSource);

    inline QsePosition *cursorPosition() const;
    void setCursorPosition(QsePosition *position);

    inline QsePosition *playPosition() const;
    void setPlayPosition(QsePosition *position);

    inline QseSelection *selection() const;
    void setSelection(QseSelection *selection);

public:
    inline SoundControlButtonBoxWidget *buttonBoxWidget() const;
    inline SoundControlSampleRateWidget *sampleRateWidget() const;
    inline SoundControlSelectionWidget *startSelectionWidget() const;
    inline SoundControlSelectionWidget *endSelectionWidget() const;
    inline SoundControlSelectionWidget *lengthSelectionWidget() const;
    inline SoundControlGeometryWidget *geometryWidget() const;

public slots:
    void setGeometry(const QseGeometry &geometry);

private:
    void createButtonBoxWidget();
    void createSampleRateWidget();
    void createSelectionWidgets();
    void createGeometryWidget();
    void createLayout();

private:
    SoundControlButtonBoxWidget *m_buttonBoxWidget;
    SoundControlSampleRateWidget *m_sampleRateWidget;
    SoundControlSelectionWidget *m_startSelectionWidget;
    SoundControlSelectionWidget *m_endSelectionWidget;
    SoundControlSelectionWidget *m_lengthSelectionWidget;
    SoundControlGeometryWidget *m_geometryWidget;
};

SoundDataSource *SoundControlPanelWidget::dataSource() const
{
    return m_startSelectionWidget->dataSource();
}

QsePosition *SoundControlPanelWidget::cursorPosition() const
{
    return m_startSelectionWidget->cursorPosition();
}

QsePosition *SoundControlPanelWidget::playPosition() const
{
    return m_startSelectionWidget->playPosition();
}

QseSelection *SoundControlPanelWidget::selection() const
{
    return m_startSelectionWidget->selection();
}

SoundControlButtonBoxWidget *SoundControlPanelWidget::buttonBoxWidget() const
{
    return m_buttonBoxWidget;
}

SoundControlSampleRateWidget *SoundControlPanelWidget::sampleRateWidget() const
{
    return m_sampleRateWidget;
}

SoundControlSelectionWidget *SoundControlPanelWidget::startSelectionWidget() const
{
    return m_startSelectionWidget;
}

SoundControlSelectionWidget *SoundControlPanelWidget::endSelectionWidget() const
{
    return m_endSelectionWidget;
}

SoundControlSelectionWidget *SoundControlPanelWidget::lengthSelectionWidget() const
{
    return m_lengthSelectionWidget;
}

SoundControlGeometryWidget *SoundControlPanelWidget::geometryWidget() const
{
    return m_geometryWidget;
}


#endif // SOUNDCONTROLPANELWIDGET_H
