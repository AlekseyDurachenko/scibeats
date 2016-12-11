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
#include "soundcontrolbuttonboxwidget.h"
#include "theme.h"
#include <QHBoxLayout>


SoundControlButtonBoxWidget::SoundControlButtonBoxWidget(QWidget *parent)
    : QWidget(parent)
{
    createActions();
    createToolButtons();
    createLayout();
}

void SoundControlButtonBoxWidget::setLoopChecked(bool checked)
{
    m_loopAction->setChecked(checked);
}

void SoundControlButtonBoxWidget::createActions()
{
    m_loopAction = new QAction(this);
    m_loopAction->setIcon(theme::icon(theme::Icon::ActionPlaybackLoop));
    m_loopAction->setText(tr("Loop"));
    m_loopAction->setCheckable(true);
    connect(m_loopAction, SIGNAL(toggled(bool)),
            this, SIGNAL(loopToggled(bool)));

    m_playAllAction = new QAction(this);
    m_playAllAction->setIcon(theme::icon(theme::Icon::ActionPlaybackPlayAll));
    m_playAllAction->setText(tr("Play All"));
    connect(m_playAllAction, SIGNAL(triggered(bool)),
            this, SIGNAL(playAllClicked()));

    m_playAction = new QAction(this);
    m_playAction->setIcon(theme::icon(theme::Icon::ActionPlaybackPlay));
    m_playAction->setText(tr("Play"));
    connect(m_playAction, SIGNAL(triggered(bool)),
            this, SIGNAL(playClicked()));

    m_pauseAction = new QAction(this);
    m_pauseAction->setIcon(theme::icon(theme::Icon::ActionPlaybackPause));
    m_pauseAction->setText(tr("Pause"));
    connect(m_pauseAction, SIGNAL(triggered(bool)),
            this, SIGNAL(pauseClicked()));

    m_stopAction = new QAction(this);
    m_stopAction->setIcon(theme::icon(theme::Icon::ActionPlaybackStop));
    m_stopAction->setText(tr("Stop"));
    connect(m_stopAction, SIGNAL(triggered(bool)),
            this, SIGNAL(stopClicked()));

    m_gotoStartAction = new QAction(this);
    m_gotoStartAction->setIcon(theme::icon(theme::Icon::ActionPlaybackGotoStart));
    m_gotoStartAction->setText(tr("Go to start"));
    connect(m_gotoStartAction, SIGNAL(triggered(bool)),
            this, SIGNAL(gotoStartClicked()));

    m_gotoEndAction = new QAction(this);
    m_gotoEndAction->setIcon(theme::icon(theme::Icon::ActionPlaybackGotoEnd));
    m_gotoEndAction->setText(tr("Go to end"));
    connect(m_gotoEndAction, SIGNAL(triggered(bool)),
            this, SIGNAL(gotoEndClicked()));
}

void SoundControlButtonBoxWidget::createToolButtons()
{
    m_loopToolButton = new QToolButton(this);
    m_loopToolButton->setDefaultAction(m_loopAction);
    m_loopToolButton->setAutoRaise(true);

    m_playAllToolButton = new QToolButton(this);
    m_playAllToolButton->setDefaultAction(m_playAllAction);
    m_playAllToolButton->setAutoRaise(true);

    m_playToolButton = new QToolButton(this);
    m_playToolButton->setDefaultAction(m_playAction);
    m_playToolButton->setAutoRaise(true);

    m_pauseToolButton = new QToolButton(this);
    m_pauseToolButton->setDefaultAction(m_pauseAction);
    m_pauseToolButton->setAutoRaise(true);

    m_stopToolButton = new QToolButton(this);
    m_stopToolButton->setDefaultAction(m_stopAction);
    m_stopToolButton->setAutoRaise(true);

    m_gotoStartToolButton = new QToolButton(this);
    m_gotoStartToolButton->setDefaultAction(m_gotoStartAction);
    m_gotoStartToolButton->setAutoRaise(true);

    m_gotoEndToolButton = new QToolButton(this);
    m_gotoEndToolButton->setDefaultAction(m_gotoEndAction);
    m_gotoEndToolButton->setAutoRaise(true);
}

void SoundControlButtonBoxWidget::createLayout()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_loopToolButton);
    hbox->addWidget(m_playAllToolButton);
    hbox->addWidget(m_playToolButton);
    hbox->addWidget(m_pauseToolButton);
    hbox->addWidget(m_stopToolButton);
    hbox->addWidget(m_gotoStartToolButton);
    hbox->addWidget(m_gotoEndToolButton);
    hbox->setContentsMargins(0, 0, 0, 0);
    hbox->setSpacing(0);
    setLayout(hbox);
}
