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
#include "csoundpropertywidget.h"
#include "ui_csoundpropertywidget.h"

CSoundPropertyWidget::CSoundPropertyWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::CSoundPropertyWidget)
{
    ui->setupUi(this);
}

CSoundPropertyWidget::~CSoundPropertyWidget()
{
    delete ui;
}

void CSoundPropertyWidget::fromSound(const CSound &sound)
{
    ui->lineEdit_title->setText(sound.property(CSound::Title));
    ui->lineEdit_artist->setText(sound.property(CSound::Artist));
    ui->lineEdit_album->setText(sound.property(CSound::Album));
    ui->lineEdit_genre->setText(sound.property(CSound::Genre));
    ui->lineEdit_date->setText(sound.property(CSound::Date));
    ui->lineEdit_tracknumber->setText(sound.property(CSound::TrackNumber));
    ui->lineEdit_software->setText(sound.property(CSound::Software));
    ui->lineEdit_copyright->setText(sound.property(CSound::Copyright));
    ui->lineEdit_license->setText(sound.property(CSound::License));
    ui->plainTextEdit_comment->setPlainText(sound.property(CSound::Comment));
}

void CSoundPropertyWidget::toSound(CSound &sound)
{
    sound.setProperty(CSound::Title, ui->lineEdit_title->text());
    sound.setProperty(CSound::Artist, ui->lineEdit_artist->text());
    sound.setProperty(CSound::Album, ui->lineEdit_album->text());
    sound.setProperty(CSound::Genre, ui->lineEdit_genre->text());
    sound.setProperty(CSound::Date, ui->lineEdit_date->text());
    sound.setProperty(CSound::TrackNumber, ui->lineEdit_tracknumber->text());
    sound.setProperty(CSound::Software, ui->lineEdit_software->text());
    sound.setProperty(CSound::Copyright, ui->lineEdit_copyright->text());
    sound.setProperty(CSound::License, ui->lineEdit_license->text());
    sound.setProperty(CSound::Comment, ui->plainTextEdit_comment->toPlainText());
}
