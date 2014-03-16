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
#include "cexportdialog.h"
#include "ui_cexportdialog.h"
#include "global.h"
#include "csndfile.h"
#include <samplerate.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

CExportDialog::CExportDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::CExportDialog)
{
    ui->setupUi(this);
    connect(ui->radioButton_customSamplerate, SIGNAL(toggled(bool)),
            this, SLOT(slot_samplerate_changed()));
    connect(ui->radioButton_originalSamplerate, SIGNAL(toggled(bool)),
            this, SLOT(slot_samplerate_changed()));
    connect(ui->checkBox_changeSpeed, SIGNAL(toggled(bool)),
            this, SLOT(slot_speed_changed()));

    slot_samplerate_changed();
    settingsRestore();
}

CExportDialog::~CExportDialog()
{
    delete ui;
}

void CExportDialog::setSound(const CSound &sound)
{
    m_sound = sound;
    ui->spinBox_originalSamplerate->setValue(sound.sampleRate());
}

void CExportDialog::setPlaySampleRate(int playSampleRate)
{
    ui->comboBox_samplerate->setCurrentIndex(
            ui->comboBox_samplerate->findText(QString::number(playSampleRate)));
}

void CExportDialog::setRange(qint64 left, qint64 right)
{
    m_left = left;
    m_right = right;
}

void CExportDialog::setSpeed(double speed)
{
    ui->doubleSpinBox_speed->setValue(speed);
}

void CExportDialog::on_toolButton_selectFileName_clicked()
{
    G_SETTINGS_INIT();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
        settings.value("LastDirectory").toString(), tr("WAV (*.wav)"));

    if (fileName.isEmpty())
        return;

    settings.setValue("LastDirectory", QFileInfo(fileName).absolutePath());
    ui->lineEdit_fileName->setText(fileName);
}

void CExportDialog::slot_samplerate_changed()
{
    bool state = ui->radioButton_customSamplerate->isChecked();
    ui->doubleSpinBox_speed->setEnabled(state);
    ui->comboBox_samplerate->setEnabled(state);
    ui->checkBox_changeSpeed->setEnabled(state);
    slot_speed_changed();
}

void CExportDialog::slot_speed_changed()
{
    ui->doubleSpinBox_speed->setEnabled(ui->checkBox_changeSpeed->isChecked() &&
            ui->radioButton_customSamplerate->isChecked());
}

void CExportDialog::accept()
{
    try
    {
        if (ui->lineEdit_fileName->text().isEmpty())
        {
            QMessageBox::information(this, tr("Information"),
                    tr("Please select the filename"));
            return;
        }

        QFile file(ui->lineEdit_fileName->text());
        if (!file.open(QIODevice::WriteOnly))
        {
            throw file.errorString();
        }

        // calculate samplerate
        int sampleRate = m_sound.sampleRate();
        if (ui->radioButton_customSamplerate->isChecked())
        {
            sampleRate = ui->comboBox_samplerate->currentText().toInt();
        }

        CSndFile sndFile(&file);
        sndFile.setChannels(1);
        sndFile.setFormat(SF_FORMAT_PCM_24|SF_FORMAT_WAV);
        sndFile.setSampleRate(sampleRate);
        sndFile.setString(SF_STR_TITLE, m_sound.property(CSound::Title));
        sndFile.setString(SF_STR_COPYRIGHT, m_sound.property(CSound::Copyright));
        sndFile.setString(SF_STR_SOFTWARE, m_sound.property(CSound::Software));
        sndFile.setString(SF_STR_ARTIST, m_sound.property(CSound::Artist));
        sndFile.setString(SF_STR_COMMENT, m_sound.property(CSound::Comment));
        sndFile.setString(SF_STR_DATE, m_sound.property(CSound::Date));
        sndFile.setString(SF_STR_ALBUM, m_sound.property(CSound::Album));
        sndFile.setString(SF_STR_LICENSE, m_sound.property(CSound::License));
        sndFile.setString(SF_STR_TRACKNUMBER, m_sound.property(CSound::TrackNumber));
        sndFile.setString(SF_STR_GENRE, m_sound.property(CSound::Genre));
        sndFile.open(SFM_WRITE);

        if (ui->checkBox_changeSpeed->isChecked())
        {
            double speed = ui->doubleSpinBox_speed->value();
            qint64 count = m_right-m_left+1;
            qint64 outdata_cnt = count/speed;
            float *outdata = new float[outdata_cnt];

            SRC_DATA src_data;
            src_data.data_in = m_sound.frames();
            src_data.input_frames = count;
            src_data.data_out = outdata;
            src_data.output_frames = outdata_cnt;
            src_data.end_of_input = 0;
            src_data.src_ratio = 1.0/speed;

            src_simple(&src_data, SRC_SINC_BEST_QUALITY, 1);
            sndFile.writef_float(outdata, outdata_cnt);

            delete []outdata;
        }
        else
        {
            sndFile.writef_float(&m_sound.frames()[m_left], m_right-m_left+1);
        }

        settingsSave();
        QDialog::accept();
    }
    catch (const QString &error)
    {
        QMessageBox::critical(this, tr("Critical"), error);
    }
}

void CExportDialog::settingsSave()
{
    G_SETTINGS_INIT();
    settings.setValue("CExportDialog/originalSamplerate",
            ui->radioButton_originalSamplerate->isChecked());
    settings.setValue("CExportDialog/changeSpeed",
            ui->checkBox_changeSpeed->isChecked());
}

void CExportDialog::settingsRestore()
{
    G_SETTINGS_INIT();
    if (settings.value("CExportDialog/originalSamplerate", false).toBool())
    {
        ui->radioButton_originalSamplerate->setChecked(true);
    }
    else
    {
        ui->radioButton_customSamplerate->setChecked(true);
    }
    ui->checkBox_changeSpeed->setChecked(
        settings.value("CExportDialog/changeSpeed", false).toBool());
}
