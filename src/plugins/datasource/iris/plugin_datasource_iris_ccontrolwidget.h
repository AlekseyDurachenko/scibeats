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
#ifndef PLUGIN_DATASOURCE_IRIS_CCONTROLWIDGET_H
#define PLUGIN_DATASOURCE_IRIS_CCONTROLWIDGET_H

#include "../plugin_datasource_cabstractwidget.h"
#include <QModelIndex>
class QtMiniSEEDDecoder;
class QNetworkAccessManager;
class QNetworkReply;

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

namespace Ui {
class CControlWidget;
}

class CItemModel;
class CAbstractItem;
class CQueryItem;
class CTraceItem;
class CControlWidget : public CAbstractWidget
{
    Q_OBJECT
    
public:
    explicit CControlWidget(QNetworkAccessManager *network,
            QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~CControlWidget();

    void abortSoundCreation();

private slots:  
    void on_horizontalSlider_minMag_valueChanged(int value);
    void on_horizontalSlider_maxRadius_valueChanged(int value);
    void on_doubleSpinBox_minMag_valueChanged(double value);
    void on_spinBox_maxRadius_valueChanged(int value);

    void slot_errorHappend(CAbstractItem *item, const QString &message);
    void slot_update_OpenOrCancel_button();
    void on_pushButton_find_clicked();
    void on_pushButton_OpenOrCancel_clicked();

    void slot_openTrace();
    void slot_reply_Finished();
    void slot_reply_downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    void saveSettings();
    void loadSettings();
    void createSound(QIODevice *stream);

private:
    Ui::CControlWidget *ui;
    QNetworkReply *m_reply;

    CItemModel *m_itemModel;
    CQueryItem *m_queryItem;
    CTraceItem *m_lastTraceItem;

};

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------

#endif // PLUGIN_DATASOURCE_IRIS_CCONTROLWIDGET_H
