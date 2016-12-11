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
#include "soundwidget.h"
#include "soundwindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>


SoundWidget::SoundWidget(QWidget *parent)
    : QWidget(parent)
{
    createPlayPosition();
    createCursorPosition();
    createSelection();
    createDataSource();
    createControlPanelWidget();
    createScrollWidget();

    createLayout();
}

void SoundWidget::createPlayPosition()
{
    m_playPosition = new QsePosition(this);
}

void SoundWidget::createCursorPosition()
{
    m_cursorPosition = new QsePosition(this);
}

void SoundWidget::createSelection()
{
    m_selection = new QseSelection(this);
}

void SoundWidget::createDataSource()
{
    m_dataSource = new SoundDataSource(this);
}

void SoundWidget::createControlPanelWidget()
{
    m_controlPanelWidget = new SoundControlPanelWidget(this);
    m_controlPanelWidget->setPlayPosition(m_playPosition);
    m_controlPanelWidget->setCursorPosition(m_cursorPosition);
    m_controlPanelWidget->setSelection(m_selection);
    m_controlPanelWidget->setDataSource(m_dataSource);
}

void SoundWidget::createScrollWidget()
{
    m_scrollWidget = new SoundScrollWidget(this);
    m_scrollWidget->setDataSource(m_dataSource);
}

void SoundWidget::createLayout()
{
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding));
    vbox->setSpacing(0);
    vbox->setContentsMargins(2, 2, 2, 2);
    vbox->addWidget(m_scrollWidget);
    QWidget *widget = new QWidget(this);
    widget->setMinimumHeight(2);
    vbox->addWidget(widget);
    vbox->addWidget(m_controlPanelWidget);
    setLayout(vbox);
}

QseWidget *SoundWidget::createSignalWidget() const
{

}

QseHorizontalAxisWidget *SoundWidget::createTimeWidget()
{

}

QseVerticalAxisWidget *SoundWidget::createAmplWidget()
{

}
