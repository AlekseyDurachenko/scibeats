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
#ifndef CEXPORTDIALOG_H
#define CEXPORTDIALOG_H

#include "csound.h"
#include <QDialog>

namespace Ui {
class CExportDialog;
}

class CExportDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CExportDialog(QWidget *parent = 0);
    virtual ~CExportDialog();

    void setSound(const CSound &sound);
    void setPlaySampleRate(int playSampleRate);
    void setRange(qint64 left, qint64 right);
    void setSpeed(double speed);

private slots:
    void on_toolButton_selectFileName_clicked();
    void slot_samplerate_changed();
    void slot_speed_changed();

protected:
    void accept();

private:
    void settingsSave();
    void settingsRestore();

private:
    Ui::CExportDialog *ui;
    CSound m_sound;
    qint64 m_left;
    qint64 m_right;

};

#endif // CEXPORTDIALOG_H
