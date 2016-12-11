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
#include "soundcontrolpanelwidget.h"
#include <QHBoxLayout>


SoundControlPanelWidget::SoundControlPanelWidget(QWidget *parent)
    : QWidget(parent)
{
    createButtonBoxWidget();
    createSampleRateWidget();
    createSelectionWidgets();
    createGeometryWidget();
    createLayout();
}

void SoundControlPanelWidget::setDataSource(SoundDataSource *dataSource)
{
    m_startSelectionWidget->setDataSource(dataSource);
    m_endSelectionWidget->setDataSource(dataSource);
    m_lengthSelectionWidget->setDataSource(dataSource);
}

void SoundControlPanelWidget::setCursorPosition(QsePosition *position)
{
    m_startSelectionWidget->setCursorPosition(position);
    m_endSelectionWidget->setCursorPosition(position);
    m_lengthSelectionWidget->setCursorPosition(position);
}

void SoundControlPanelWidget::setPlayPosition(QsePosition *position)
{
    m_startSelectionWidget->setPlayPosition(position);
    m_endSelectionWidget->setPlayPosition(position);
    m_lengthSelectionWidget->setPlayPosition(position);
}

void SoundControlPanelWidget::setSelection(QseSelection *selection)
{
    m_startSelectionWidget->setSelection(selection);
    m_endSelectionWidget->setSelection(selection);
    m_lengthSelectionWidget->setSelection(selection);
}

void SoundControlPanelWidget::setGeometry(const QseGeometry &geometry)
{
    m_geometryWidget->setGeometry(geometry);
}

void SoundControlPanelWidget::createButtonBoxWidget()
{
    m_buttonBoxWidget = new SoundControlButtonBoxWidget(this);
}

void SoundControlPanelWidget::createSampleRateWidget()
{
    m_sampleRateWidget = new SoundControlSampleRateWidget(this);
}

void SoundControlPanelWidget::createSelectionWidgets()
{
    m_startSelectionWidget = new SoundControlSelectionWidget(this);
    m_startSelectionWidget->setSection(SoundControlSelectionWidget::StartSection);
    m_startSelectionWidget->setRepresentation(SoundControlSelectionWidget::TimeRepresentation);
    connect(m_sampleRateWidget, SIGNAL(valueChanged(int)),
            m_startSelectionWidget, SLOT(setSapleRate(int)));

    m_endSelectionWidget = new SoundControlSelectionWidget(this);
    m_endSelectionWidget->setSection(SoundControlSelectionWidget::EndSection);
    m_endSelectionWidget->setRepresentation(SoundControlSelectionWidget::TimeRepresentation);
    connect(m_sampleRateWidget, SIGNAL(valueChanged(int)),
            m_endSelectionWidget, SLOT(setSapleRate(int)));

    m_lengthSelectionWidget = new SoundControlSelectionWidget(this);
    m_lengthSelectionWidget->setSection(SoundControlSelectionWidget::LengthSection);
    m_lengthSelectionWidget->setRepresentation(SoundControlSelectionWidget::TimeRepresentation);
    connect(m_sampleRateWidget, SIGNAL(valueChanged(int)),
            m_lengthSelectionWidget, SLOT(setSapleRate(int)));
}

void SoundControlPanelWidget::createGeometryWidget()
{
    m_geometryWidget = new SoundControlGeometryWidget(this);
}

void SoundControlPanelWidget::createLayout()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_buttonBoxWidget);
    hbox->addWidget(m_sampleRateWidget);
    hbox->addWidget(m_startSelectionWidget);
    hbox->addWidget(m_endSelectionWidget);
    hbox->addWidget(m_lengthSelectionWidget);
    hbox->addWidget(m_geometryWidget);
    hbox->setContentsMargins(0, 0, 0, 0);
    setLayout(hbox);
}
