// Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef PLUGIN_PLAYBACKEND_PORTAUDIO_CCONFIGWIDGET_H
#define PLUGIN_PLAYBACKEND_PORTAUDIO_CCONFIGWIDGET_H

#include "../plugin_playbackend_cabstractconfigwidget.h"

// ----------------------------------------------------------------------
namespace plugin {
namespace playbackend {
namespace portaudio {
// ----------------------------------------------------------------------

class CConfigWidget : public CAbstractConfigWidget
{
    Q_OBJECT
public:
    explicit CConfigWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    QVariant configuraiton() const;

public slots:
    void setConfiguration(const QVariant &configuration);

};

// ----------------------------------------------------------------------
} // namespace portaudio
} // namespace playbackend
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_PLAYBACKEND_PORTAUDIO_CCONFIGWIDGET_H
