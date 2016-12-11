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
#ifndef SOUNDWINDOW_H
#define SOUNDWINDOW_H


#include "soundwriter.h"
#include "soundpeakscalculator.h"
#include "soundwidget.h"
#include "soundproviderrequest.h"
#include "soundprovider.h"
#include "soundrequestlogwidget.h"
#include "logmessageitemmodel.h"
#include <QMdiSubWindow>


class SoundWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SoundWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~SoundWindow() final;

    inline SoundWriter *soundWriter() const;
    void setSoundWriter(SoundWriter *soundWriter);

    inline SoundProvider *soundProvider() const;
    void setSoundProvider(SoundProvider *soundProvider);

    inline SoundPeaksCalculator *soundPeaksCalculator() const;
    void setSoundPeaksCalculator(SoundPeaksCalculator *soundPeaksCalculator);

    inline const SoundProviderRequest &request() const;
    void setRequest(const SoundProviderRequest &request);

public slots:
    void openSound();
    void showSound();

private slots:
    void requestLogWidgetButtonAbort();
    void requestLogWidgetButtonRetry();
    void requestLogWidgetButtonClose();
    void replyProgress(double value, double total);
    void replyFinished();

private:
    void createLogMessageManager();

private:
    Sound m_sound;
    SoundPeaks m_soundPeaks;

    SoundProviderRequest m_soundProviderRequest;
    SoundProviderReply *m_soundProviderReply;

    SoundPeaksReply *m_soundPeaksReply;

    SoundRequestLogWidget *m_requestLogWidget;
    SoundWidget *m_soundWidget;

    LogMessageManager *m_logMgr;

private:
    SoundWriter *m_soundWriter;
    SoundProvider *m_soundProvider;
    SoundPeaksCalculator *m_soundPeaksCalculator;
};

SoundWriter *SoundWindow::soundWriter() const
{
    return m_soundWriter;
}

SoundProvider *SoundWindow::soundProvider() const
{
    return m_soundProvider;
}

SoundPeaksCalculator *SoundWindow::soundPeaksCalculator() const
{
    return m_soundPeaksCalculator;
}

const SoundProviderRequest &SoundWindow::request() const
{
    return m_soundProviderRequest;
}


#endif // SOUNDWINDOW_H
