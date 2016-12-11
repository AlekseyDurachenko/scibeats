// Copyright 2015-2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#ifndef THEME_H
#define THEME_H


#include <QHash>
#include <QIcon>


// ----------------------------------------------------------------------
namespace theme {
// ----------------------------------------------------------------------


enum class Icon : int {
    App,

    ActionPlaybackGotoStart,
    ActionPlaybackGotoEnd,
    ActionPlaybackLoop,
    ActionPlaybackPause,
    ActionPlaybackPlayAll,
    ActionPlaybackPlay,
    ActionPlaybackStop,

    ActionZoomIn,
    ActionZoomOut,
    ActionZoomAllHorizontal,
    ActionZoomAllVertical,

    ActionQuit,

    ActionAbout,
    ActionAboutQt
};


inline uint qHash(Icon key)
{
    return ::qHash(static_cast<uint>(key));
}


void init();
const QIcon icon(Icon name, const QIcon &defaultIcon = QIcon());
const QImage &appImage();


// ----------------------------------------------------------------------
} // namespace theme
// ----------------------------------------------------------------------


#endif // THEME_H
