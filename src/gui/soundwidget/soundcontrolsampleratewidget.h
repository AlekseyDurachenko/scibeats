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
#ifndef SOUNDCONTROLSAMPLERATEWIDGET_H
#define SOUNDCONTROLSAMPLERATEWIDGET_H


#include <QWidget>
#include <QSpinBox>
#include <QSlider>


class SoundControlSampleRateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundControlSampleRateWidget(QWidget *parent = 0);

    inline int value() const;
    inline int minimum() const;
    inline int maximum() const;
    void setRange(int minimum, int maximum);

signals:
    void valueChanged(int value);

public slots:
    void setValue(int value);

private:
    QSpinBox *m_spinBox;
    QSlider *m_slider;
};

int SoundControlSampleRateWidget::value() const
{
    return m_spinBox->value();
}

int SoundControlSampleRateWidget::minimum() const
{
    return m_spinBox->minimum();
}

int SoundControlSampleRateWidget::maximum() const
{
    return m_spinBox->maximum();
}



#endif // SOUNDCONTROLSAMPLERATEWIDGET_H
