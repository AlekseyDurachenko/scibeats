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
#ifndef SOUNDCONTROLBUTTONBOXWIDGET_H
#define SOUNDCONTROLBUTTONBOXWIDGET_H


#include <QWidget>
#include <QAction>
#include <QToolButton>


class SoundControlButtonBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundControlButtonBoxWidget(QWidget *parent = 0);

    inline bool isLoopChecked();

signals:
    void loopToggled(bool checked);
    void playAllClicked();
    void playClicked();
    void pauseClicked();
    void stopClicked();
    void gotoStartClicked();
    void gotoEndClicked();

public slots:
    void setLoopChecked(bool checked);

private:
    void createActions();
    void createToolButtons();
    void createLayout();

private:
    QAction *m_loopAction;
    QAction *m_playAllAction;
    QAction *m_playAction;
    QAction *m_pauseAction;
    QAction *m_stopAction;
    QAction *m_gotoStartAction;
    QAction *m_gotoEndAction;

    QToolButton *m_loopToolButton;
    QToolButton *m_playAllToolButton;
    QToolButton *m_playToolButton;
    QToolButton *m_pauseToolButton;
    QToolButton *m_stopToolButton;
    QToolButton *m_gotoStartToolButton;
    QToolButton *m_gotoEndToolButton;
};

bool SoundControlButtonBoxWidget::isLoopChecked()
{
    return m_loopAction->isChecked();
}


#endif // SOUNDCONTROLBUTTONBOXWIDGET_H
