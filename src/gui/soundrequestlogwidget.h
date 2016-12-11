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
#ifndef SOUNDREQUESTLOGWIDGET_H
#define SOUNDREQUESTLOGWIDGET_H


#include "logmessagemanager.h"
#include "logmessageitemmodel.h"
#include "soundproviderrequest.h"
#include <QWidget>


namespace Ui {
class SoundRequestLogWidget;
}


class SoundRequestLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoundRequestLogWidget(LogMessageManager *logMgr, QWidget *parent = 0);
    virtual ~SoundRequestLogWidget() final;

    void setRequest(const SoundProviderRequest &request);
    SoundProviderRequest toRequest() const;

    void switchToRequestTab();
    void switchToLogTab();

    void setRetryButtonVisible(bool visible);
    void setAbortButtonVisible(bool visible);
    void setProgress(double value, double total);

signals:
    void buttonAbortClicked();
    void buttonRetryClicked();
    void buttonCloseClicked();

private slots:
    void on_abort_pushButton_clicked();
    void on_retry_pushButton_clicked();
    void on_close_pushButton_clicked();
    void on_requestSave_toolButton_clicked();
    void on_logSave_toolButton_clicked();
    void on_logClear_toolButton_clicked();

private:
    Ui::SoundRequestLogWidget *ui;
};


#endif // SOUNDREQUESTLOGWIDGET_H
