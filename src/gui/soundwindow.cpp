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
#include "soundwindow.h"
#include "mainwindow.h"
#include "soundrequestlogwidget.h"


SoundWindow::SoundWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMdiSubWindow(parent, flags)
{
    setAttribute(Qt::WA_DeleteOnClose);

    m_soundProviderReply = nullptr;
    m_soundPeaksReply = nullptr;

    m_soundWriter = nullptr;
    m_soundProvider = nullptr;
    m_soundPeaksCalculator = nullptr;

    m_requestLogWidget = nullptr;
    m_soundWidget = nullptr;

    createLogMessageManager();
}

SoundWindow::~SoundWindow()
{
}

void SoundWindow::setSoundWriter(SoundWriter *soundWriter)
{
    if (m_soundWriter != soundWriter) {
        m_soundWriter = soundWriter;
    }
}

void SoundWindow::setSoundProvider(SoundProvider *soundProvider)
{
    if (m_soundProvider != soundProvider) {
        m_soundProvider = soundProvider;
    }
}

void SoundWindow::setSoundPeaksCalculator(SoundPeaksCalculator *soundPeaksCalculator)
{
    if (m_soundPeaksCalculator != soundPeaksCalculator) {
        m_soundPeaksCalculator = soundPeaksCalculator;
    }
}

void SoundWindow::setRequest(const SoundProviderRequest &request)
{
    if (m_soundProviderRequest != request) {
        m_soundProviderRequest = request;
    }
}

void SoundWindow::openSound()
{
    Q_ASSERT(soundProvider());
    Q_ASSERT(m_soundProviderReply == nullptr);

    if (m_soundWidget) {
        delete m_soundWidget;
        m_soundWidget = nullptr;
    }

    if (!m_requestLogWidget) {
        m_requestLogWidget = new SoundRequestLogWidget(m_logMgr, this);
        connect(m_requestLogWidget, SIGNAL(buttonAbortClicked()),
                this, SLOT(requestLogWidgetButtonAbort()));
        connect(m_requestLogWidget, SIGNAL(buttonRetryClicked()),
                this, SLOT(requestLogWidgetButtonRetry()));
        connect(m_requestLogWidget, SIGNAL(buttonCloseClicked()),
                this, SLOT(requestLogWidgetButtonClose()));
        setWidget(m_requestLogWidget);
    }

    QString errmsg;
    m_soundProviderReply = soundProvider()->get(m_soundProviderRequest, &errmsg);
    if (!m_soundProviderReply) {
        m_logMgr->append(LogMessage(LogMessage::Critical, errmsg));
        m_requestLogWidget->setRetryButtonVisible(true);
        m_requestLogWidget->setAbortButtonVisible(false);
        m_requestLogWidget->setProgress(1.0, 1.0);
    }
    else {
        connect(m_soundProviderReply, SIGNAL(progress(double, double)),
                this, SLOT(replyProgress(double, double)));
        connect(m_soundProviderReply, SIGNAL(progress(LogMessage)),
                m_logMgr, SLOT(append(LogMessage)));
        connect(m_soundProviderReply, SIGNAL(finished()),
                this, SLOT(replyFinished()));
        m_requestLogWidget->setRetryButtonVisible(false);
        m_requestLogWidget->setAbortButtonVisible(true);
    }

    m_requestLogWidget->switchToLogTab();
    m_requestLogWidget->setRequest(request());
}

void SoundWindow::showSound()
{
    if (m_requestLogWidget) {
        delete m_requestLogWidget;
        m_requestLogWidget = nullptr;
    }

    if (!m_soundWidget) {
        m_soundWidget = new SoundWidget(this);
        setWidget(m_soundWidget);
    }
}

void SoundWindow::requestLogWidgetButtonAbort()
{
    if (m_soundProviderReply) {
        m_soundProviderReply->abort();
    }

    if (m_soundPeaksReply) {
        m_soundPeaksReply->abort();
    }
}

void SoundWindow::requestLogWidgetButtonRetry()
{
    if (m_soundProviderReply) {
        setRequest(m_requestLogWidget->toRequest());
        openSound();
    }
}

void SoundWindow::requestLogWidgetButtonClose()
{
    if (m_soundProviderReply) {
        m_soundProviderReply->abort();
        m_soundProviderReply->deleteLater();
    }

    if (m_soundPeaksReply) {
        m_soundPeaksReply->abort();
        m_soundPeaksReply->deleteLater();
    }

    close();
}

void SoundWindow::replyProgress(double value, double total)
{
    if (m_requestLogWidget) {
        if (m_soundProviderReply) {
            const double newValue = (value / total) * 0.8;
            m_requestLogWidget->setProgress(newValue, 1.0);
        }
        if (m_soundPeaksReply) {
            const double newValue = (value / total) * 0.2;
            m_requestLogWidget->setProgress(0.8 + newValue, 1.0);
        }
    }
}

void SoundWindow::replyFinished()
{
    if (m_requestLogWidget) {
        m_requestLogWidget->switchToLogTab();
    }

    if (m_soundProviderReply) {
        if (m_soundProviderReply->resultState() == SoundProviderResultState::Success) {
            m_sound = m_soundProviderReply->sound();

            m_soundProviderReply->deleteLater();
            m_soundProviderReply = nullptr;

            SoundPeaksRequest request;
            request.setSound(m_sound);

            m_soundPeaksReply = m_soundPeaksCalculator->calc(request);
            connect(m_soundPeaksReply, SIGNAL(progress(double, double)),
                    this, SLOT(replyProgress(double, double)));
            connect(m_soundPeaksReply, SIGNAL(finished()),
                    this, SLOT(replyFinished()));

            m_logMgr->append(LogMessage(LogMessage::Information, tr("start peaks calculation")));
        }
        else {
            m_soundProviderReply->deleteLater();
            m_soundProviderReply = nullptr;
        }

        return;
    }

    if (m_soundPeaksReply) {
        if (m_soundPeaksReply->resultState() == SoundPeaksResultState::Success) {
            m_soundPeaks = m_soundPeaksReply->soundPeaks();
            m_logMgr->append(LogMessage(LogMessage::Information, tr("peaks calculated")));

            m_soundPeaksReply->deleteLater();
            m_soundPeaksReply = nullptr;

            m_requestLogWidget->setRetryButtonVisible(false);
            m_requestLogWidget->setAbortButtonVisible(false);

            showSound();
        }
        else {
            m_soundPeaksReply->deleteLater();
            m_soundPeaksReply = nullptr;

            m_requestLogWidget->setRetryButtonVisible(true);
            m_requestLogWidget->setAbortButtonVisible(false);
        }

        return;
    }
}

void SoundWindow::createLogMessageManager()
{
    m_logMgr = new LogMessageManager(this);
}
