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
#ifndef SOUNDCONTROLGEOMETRYWIDGET_H
#define SOUNDCONTROLGEOMETRYWIDGET_H


#include "qsegeometry.h"
#include <QLabel>


class SoundControlGeometryWidget : public QLabel
{
    Q_OBJECT
public:
    explicit SoundControlGeometryWidget(QWidget *parent = 0);
    inline const QseGeometry &geometry() const;

public slots:
    void setGeometry(const QseGeometry &geometry);

private:
    void updateWidget();

private:
    QseGeometry m_geometry;
};

const QseGeometry &SoundControlGeometryWidget::geometry() const
{
    return m_geometry;
}


#endif // SOUNDCONTROLGEOMETRYWIDGET_H
