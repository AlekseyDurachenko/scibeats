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
#ifndef CDATASOURCEWIDGET_H
#define CDATASOURCEWIDGET_H

#include "csound.h"
#include <QWidget>
#include <QNetworkAccessManager>

namespace Ui {
class CDataSourceWidget;
}

class CDataSourceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CDataSourceWidget(QNetworkAccessManager *network,
            QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CDataSourceWidget();

signals:
    void soundCreated(const CSound& sound);

private slots:
    void slot_aboutSoundToBeCreated();

private:
    Ui::CDataSourceWidget *ui;

};

#endif // CDATASOURCEWIDGET_H
