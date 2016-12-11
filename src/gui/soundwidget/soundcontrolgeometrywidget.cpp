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
#include "soundcontrolgeometrywidget.h"
#include <QFontDatabase>


SoundControlGeometryWidget::SoundControlGeometryWidget(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);

#if QT_VERSION >= 0x050000
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
#else
    QFont fixedFont;
    fixedFont.setStyleHint(QFont::Monospace);
#endif
    const QFontMetrics fontMetrics(fixedFont);
    setMinimumWidth(fontMetrics.width("0:000000000"));

    updateWidget();
}

void SoundControlGeometryWidget::setGeometry(const QseGeometry &geometry)
{
    if (m_geometry != geometry) {
        m_geometry = geometry;
        updateGeometry();
    }
}

void SoundControlGeometryWidget::updateWidget()
{
    if (m_geometry.spp() > 0) {
        setText(QString("1:%1").arg(QString::number(m_geometry.spp())));
    }
    else {
        setText(QString("%1:1").arg(QString::number(-m_geometry.spp())));
    }
}
