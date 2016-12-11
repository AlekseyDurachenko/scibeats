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
#include "theme.h"
#include "resources.h"
#include <QDir>


static QHash<theme::Icon, QIcon> m_icons;
static QImage m_appImage;


static QString themeFileName(const QString &baseFileName)
{
    return iconFileName(baseFileName + ".png");
}

static QIcon themeIcon(const QString &baseFileName)
{
    return QIcon(themeFileName(baseFileName));
}

static QImage themeImage(const QString &baseFileName)
{
    return QImage(themeFileName(baseFileName));
}


void theme::init()
{
    m_appImage = themeImage("scibeats");

    m_icons.insert(Icon::App, themeIcon("scibeats"));

    m_icons.insert(Icon::ActionPlaybackGotoStart, themeIcon("action-playback-gotostart"));
    m_icons.insert(Icon::ActionPlaybackGotoEnd, themeIcon("action-playback-gotoend"));
    m_icons.insert(Icon::ActionPlaybackLoop, themeIcon("action-playback-loop"));
    m_icons.insert(Icon::ActionPlaybackPause, themeIcon("action-playback-pause"));
    m_icons.insert(Icon::ActionPlaybackPlay, themeIcon("action-playback-play"));
    m_icons.insert(Icon::ActionPlaybackPlayAll, themeIcon("action-playback-playall"));
    m_icons.insert(Icon::ActionPlaybackStop, themeIcon("action-playback-stop"));

    m_icons.insert(Icon::ActionZoomIn, themeIcon("action-zoom-in"));
    m_icons.insert(Icon::ActionZoomOut, themeIcon("action-zoom-out"));
    m_icons.insert(Icon::ActionZoomAllHorizontal, themeIcon("action-zoom-all-horizontal"));
    m_icons.insert(Icon::ActionZoomAllVertical, themeIcon("action-zoom-all-vertical"));

    m_icons.insert(Icon::ActionQuit, themeIcon("action-quit"));

    m_icons.insert(Icon::ActionAbout,     themeIcon("action-about"));
    m_icons.insert(Icon::ActionAboutQt,   themeIcon("action-about-qt"));
}

const QIcon theme::icon(Icon name, const QIcon &defaultIcon)
{
    return m_icons.value(name, defaultIcon);
}

const QImage &theme::appImage()
{
    return m_appImage;
}
