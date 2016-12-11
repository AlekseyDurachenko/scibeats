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
#include "soundrequestlogwidget.h"
#include "ui_soundrequestlogwidget.h"
#include <QDebug>


SoundRequestLogWidget::SoundRequestLogWidget(LogMessageManager *logMgr, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SoundRequestLogWidget)
{
    Q_ASSERT(logMgr);

    ui->setupUi(this);
    ui->log_treeView->setModel(new LogMessageItemModel(logMgr, this));
}

SoundRequestLogWidget::~SoundRequestLogWidget()
{
    delete ui;
}

void SoundRequestLogWidget::setRequest(const SoundProviderRequest &request)
{
    ui->request_textEdit->setPlainText(QString::fromUtf8(request.query()));
}

SoundProviderRequest SoundRequestLogWidget::toRequest() const
{
    SoundProviderRequest request;
    request.setQuery(ui->request_textEdit->toPlainText().toUtf8());
    return request;
}

void SoundRequestLogWidget::switchToRequestTab()
{
    ui->tabWidget->setCurrentIndex(0);
}

void SoundRequestLogWidget::switchToLogTab()
{
    ui->tabWidget->setCurrentIndex(1);
}

void SoundRequestLogWidget::setRetryButtonVisible(bool visible)
{
    ui->retry_pushButton->setVisible(visible);
}

void SoundRequestLogWidget::setAbortButtonVisible(bool visible)
{
    ui->abort_pushButton->setVisible(visible);
}

void SoundRequestLogWidget::setProgress(double value, double total)
{
    ui->progressBar->setRange(0, 1000);
    ui->progressBar->setValue(qRound((value / total) * 1000));
}

void SoundRequestLogWidget::on_abort_pushButton_clicked()
{
    emit buttonAbortClicked();
}

void SoundRequestLogWidget::on_retry_pushButton_clicked()
{
    emit buttonRetryClicked();
}

void SoundRequestLogWidget::on_close_pushButton_clicked()
{
    emit buttonCloseClicked();
}

void SoundRequestLogWidget::on_requestSave_toolButton_clicked()
{
    // TODO: realize me
}

void SoundRequestLogWidget::on_logSave_toolButton_clicked()
{
    // TODO: realize me
}

void SoundRequestLogWidget::on_logClear_toolButton_clicked()
{
    // TODO: realize me
}
