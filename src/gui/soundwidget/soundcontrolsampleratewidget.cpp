// Copyright 2016, Durachenko Aleksey V. <durachenko@gs.sbras.ru>
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
#include "soundcontrolsampleratewidget.h"
#include <QHBoxLayout>
#include <QLabel>


SoundControlSampleRateWidget::SoundControlSampleRateWidget(QWidget *parent)
    : QWidget(parent)
{
    // Design:
    // |<label> <slider[expanded]> <spinBox>|

    QLabel *label = new QLabel(this);
    label->setText(tr("Rate (Hz)"));

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setSizePolicy(QSizePolicy::Expanding, m_slider->sizePolicy().verticalPolicy());
    m_slider->setMinimumWidth(80);

    m_spinBox = new QSpinBox(this);
    m_spinBox->setSizePolicy(m_spinBox->sizePolicy().horizontalPolicy(), QSizePolicy::Minimum);
    m_spinBox->setSuffix(tr(" hz"));
    m_spinBox->setAlignment(Qt::AlignRight);
    m_spinBox->setSingleStep(100);

    setRange(100, 192000);
    setValue(44100);

    connect(m_spinBox, SIGNAL(valueChanged(int)),
            m_slider, SLOT(setValue(int)));
    connect(m_slider, SIGNAL(valueChanged(int)),
            m_spinBox, SLOT(setValue(int)));
    connect(m_spinBox, SIGNAL(valueChanged(int)),
            this, SIGNAL(valueChanged(int)));

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(label);
    hbox->addWidget(m_slider);
    hbox->addWidget(m_spinBox);
    hbox->setContentsMargins(0, 0, 0, 0);
    setLayout(hbox);
}

void SoundControlSampleRateWidget::setRange(int minimum, int maximum)
{
    m_spinBox->setRange(minimum, maximum);
    m_slider->setRange(minimum, maximum);
}

void SoundControlSampleRateWidget::setValue(int value)
{
    m_spinBox->setValue(value);
    m_slider->setValue(value);
}
