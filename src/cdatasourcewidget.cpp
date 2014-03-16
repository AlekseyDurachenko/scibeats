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
#include "cdatasourcewidget.h"
#include "ui_cdatasourcewidget.h"
#include "plugins/datasource/plugin_datasource_factory.h"
#include <QDebug>
using namespace plugin::datasource;

CDataSourceWidget::CDataSourceWidget(QNetworkAccessManager *network,
        QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f),
        ui(new Ui::CDataSourceWidget)
{
    ui->setupUi(this);

    // create the plugin widgets and associate them with the indexes of combobox
    QList<QByteArray> keys = plugin::datasource::Factory::ids();
    for (int i = 0; i < keys.count(); ++i)
    {       
        CAbstractWidget *widget = Factory::create(keys.at(i),
                network, ui->stackedWidget_plugin);
        connect(widget, SIGNAL(aboutSoundToBeCreated()),
                this, SLOT(slot_aboutSoundToBeCreated()));
        connect(widget, SIGNAL(soundCreated(CSound)),
                this, SIGNAL(soundCreated(CSound)));

        ui->stackedWidget_plugin->addWidget(widget);
        ui->comboBox_plugin->addItem(Factory::icon(keys.at(i)),
                Factory::name(keys.at(i)));
        ui->comboBox_plugin->setItemData(i, Factory::desc(keys.at(i)),
                Qt::ToolTipRole);

    }
    if (keys.count() > 0)
    {
        ui->comboBox_plugin->setCurrentIndex(0);
        ui->stackedWidget_plugin->setCurrentIndex(0);
    }
}

CDataSourceWidget::~CDataSourceWidget()
{
    delete ui;
}

void CDataSourceWidget::slot_aboutSoundToBeCreated()
{
    for (int i = 0; i < ui->stackedWidget_plugin->count(); ++i)
    {
        if (sender() != ui->stackedWidget_plugin->widget(i))
        {
            qobject_cast<CAbstractWidget *>(
                    ui->stackedWidget_plugin->widget(i))->abortSoundCreation();
        }
    }
}
