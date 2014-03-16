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
#include "plugin_datasource_iris_ccontrolwidget.h"
#include "ui_plugin_datasource_iris_ccontrolwidget.h"
#include "plugin_datasource_iris_citemmodel.h"
#include "plugin_datasource_iris_ctraceitem.h"
#include "plugin_datasource_iris_ceventitem.h"
#include "plugin_datasource_iris_cqueryitem.h"
#include "global.h"
#include <QtMiniSEEDDecoder>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QDebug>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CControlWidget::CControlWidget(QNetworkAccessManager *net,
        QWidget *parent, Qt::WindowFlags f) :
        CAbstractWidget(net, parent, f), ui(new Ui::CControlWidget)
{
    ui->setupUi(this);    
    // should be allocated in host app
    m_reply = 0;

    m_queryItem = new CQueryItem(network(), this);
    connect(m_queryItem, SIGNAL(errorHappend(CAbstractItem*,QString)),
            this, SLOT(slot_errorHappend(CAbstractItem*,QString)));
    m_itemModel = new CItemModel(this);
    m_itemModel->setRootItem(m_queryItem);
    ui->treeView->setModel(m_itemModel);
    connect(ui->treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slot_update_OpenOrCancel_button()));
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(slot_openTrace()));

    loadSettings();
    slot_update_OpenOrCancel_button();
}

CControlWidget::~CControlWidget()
{
    saveSettings();
    delete ui;
}

void CControlWidget::abortSoundCreation()
{
    if (m_reply)
    {
        m_reply->abort();
        slot_update_OpenOrCancel_button();
    }
}

void CControlWidget::on_horizontalSlider_minMag_valueChanged(int value)
{
    ui->doubleSpinBox_minMag->setValue(value / 10.0);
}

void CControlWidget::on_horizontalSlider_maxRadius_valueChanged(int value)
{
    ui->spinBox_maxRadius->setValue(value);
}

void CControlWidget::on_doubleSpinBox_minMag_valueChanged(double value)
{
    ui->horizontalSlider_minMag->setValue(static_cast<int>(value * 10.0));
}

void CControlWidget::on_spinBox_maxRadius_valueChanged(int value)
{
    ui->horizontalSlider_maxRadius->setValue(value);
}

void CControlWidget::slot_errorHappend(CAbstractItem *item,
        const QString &message)
{
    QMessageBox::warning(this, tr("Something happend..."), message);

    // need fix this code.
    // TODO:
    // * createo custom QTreeView
    // * create correct collaps/expand association with CQueryItem
    //   (expand - download data, if error collaps and clean state of the
    //    CEventItem)
    if (item->itemType() == CAbstractItem::Event)
    {
        ui->treeView->collapse(m_itemModel->index(item->row(), 0, QModelIndex()));
    }
}

void CControlWidget::slot_update_OpenOrCancel_button()
{
    if (m_reply)
    {
        ui->pushButton_OpenOrCancel->setText(tr("Cancel"));
        ui->pushButton_OpenOrCancel->setEnabled(true);
    }
    else
    {
        ui->pushButton_OpenOrCancel->setText(tr("Open"));
        if (ui->treeView->currentIndex().isValid() && static_cast<CAbstractItem *>
                (ui->treeView->currentIndex().internalPointer())->itemType() == CAbstractItem::Trace)
        {
            ui->pushButton_OpenOrCancel->setEnabled(true);
        }
        else
        {
            ui->pushButton_OpenOrCancel->setEnabled(false);
        }
    }
}

void CControlWidget::on_pushButton_find_clicked()
{
    m_queryItem->setMinMagnitude(ui->doubleSpinBox_minMag->value());
    m_queryItem->setMaxRadiusKm(ui->spinBox_maxRadius->value());
    m_queryItem->setStartDateTime(QDateTime(ui->dateEdit_from->date(), QTime(), Qt::UTC));
    m_queryItem->setEndDateTime(QDateTime(ui->dateEdit_to->date(), QTime(), Qt::UTC));
    m_queryItem->execute();
}

void CControlWidget::on_pushButton_OpenOrCancel_clicked()
{
    if (m_reply)
    {
        m_reply->abort();
    }
    else
    {
        slot_openTrace();
    }

    slot_update_OpenOrCancel_button();
}

void CControlWidget::slot_openTrace()
{
    if (static_cast<CAbstractItem *>(ui->treeView->currentIndex().internalPointer())
            ->itemType() == CAbstractItem::Trace)
    {
        if (m_reply)
        {
            m_reply->abort();
        }
        m_lastTraceItem = static_cast<CTraceItem *>(ui->treeView->currentIndex().internalPointer());

        // create new query
        QUrl queryUrl("http://service.iris.edu/irisws/timeseries/1/query");
        queryUrl.addQueryItem("output", "miniseed");
        queryUrl.addQueryItem("net", m_lastTraceItem->fdsn());
        queryUrl.addQueryItem("sta", m_lastTraceItem->station());
        queryUrl.addQueryItem("loc", m_lastTraceItem->location());
        queryUrl.addQueryItem("cha", m_lastTraceItem->channel());
        queryUrl.addQueryItem("start", m_lastTraceItem->phaseDateTime()
                              .addSecs(-ui->spinBox_secondBefore->value()).toString("yyyy-MM-ddTHH:mm:ss"));
        queryUrl.addQueryItem("end", m_lastTraceItem->phaseDateTime()
                              .addSecs(ui->spinBox_secondAfter->value()).toString("yyyy-MM-ddTHH:mm:ss"));

        // threre should be log output
        qDebug() << queryUrl.toString();

        // show progress bar
        m_reply = network()->get(QNetworkRequest(queryUrl));
        connect(m_reply, SIGNAL(finished()), this, SLOT(slot_reply_Finished()));
        connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
                this, SLOT(slot_reply_downloadProgress(qint64,qint64)));
        emit aboutSoundToBeCreated();
    }
}

void CControlWidget::slot_reply_Finished()
{
    if (m_reply->error() == QNetworkReply::NoError)
    {
        createSound(m_reply);
        ui->progressBar->setValue(100);
    }
    else if (m_reply->error() == QNetworkReply::ContentNotFoundError)
    {
        m_lastTraceItem->setDataFound(false);
        ui->progressBar->setValue(0);
        QMessageBox::warning(this, tr("Not found"), tr("The trace are not avaible on the server-side"));
    }
    else if (m_reply->error() != QNetworkReply::OperationCanceledError)
    {
        ui->progressBar->setValue(0);
        QMessageBox::warning(this, tr("Something happend..."), m_reply->errorString());
    }
    else
    {
        ui->progressBar->setValue(0);
    }

    m_reply->deleteLater();
    m_reply = 0;

    slot_update_OpenOrCancel_button();
}

void CControlWidget::slot_reply_downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    ui->progressBar->setValue(static_cast<int>(bytesReceived * 100.0 / bytesTotal));
}

void CControlWidget::saveSettings()
{
    G_SETTINGS_INIT();
    settings.setValue("plugin/datasource/iris/MinMagnitude", ui->doubleSpinBox_minMag->value());
    settings.setValue("plugin/datasource/iris/MaxRadius", ui->spinBox_maxRadius->value());
    settings.setValue("plugin/datasource/iris/DateFrom", ui->dateEdit_from->date());
    settings.setValue("plugin/datasource/iris/DateTo", ui->dateEdit_to->date());
    settings.setValue("plugin/datasource/iris/SecBefore", ui->spinBox_secondBefore->value());
    settings.setValue("plugin/datasource/iris/SecAfter", ui->spinBox_secondAfter->value());
    settings.setValue("plugin/datasource/iris/TreeView/header/state", ui->treeView->header()->saveState());
}

void CControlWidget::loadSettings()
{
    G_SETTINGS_INIT();
    ui->doubleSpinBox_minMag->setValue(settings.value("plugin/datasource/iris/MinMagnitude", 7.0).toDouble());
    ui->spinBox_maxRadius->setValue(settings.value("plugin/datasource/iris/MaxRadius", 8000).toInt());
    ui->dateEdit_from->setDate(settings.value("plugin/datasource/iris/DateFrom", QDate(2000,1,1)).toDate());
    ui->dateEdit_to->setDate(settings.value("plugin/datasource/iris/DateTo", QDate::currentDate()).toDate());
    ui->spinBox_secondBefore->setValue(settings.value("plugin/datasource/iris/SecBefore", 300).toInt());
    ui->spinBox_secondAfter->setValue(settings.value("plugin/datasource/iris/SecAfter", 3600).toInt());
    ui->treeView->header()->restoreState(settings.value("plugin/datasource/iris/TreeView/header/state",
            ui->treeView->header()->saveState()).toByteArray());
}

void CControlWidget::createSound(QIODevice *stream)
{
    QList<QtMiniSEEDRecord *> recordList;
    QtMiniSEEDDecoder miniseedDecoder;
    int totalFrameCount = 0;

    // 1: Read all records from the stream and calculate total samples
    //    we ignore the gap/overlap in the original trace
    QtMiniSEEDRecord *rec;
    while ((rec = miniseedDecoder.readRecord(stream)))
    {
        totalFrameCount += rec->sampleCount();
        recordList.push_back(rec);
    }

    if (totalFrameCount > 0)
    {
        // 2: create sound object
        CSound snd;
        snd.setSampleRate(m_lastTraceItem->sampleRate());
        snd.resize(totalFrameCount);
        snd.setProperty(CSound::Software, "Qarthbeat");
        snd.setProperty(CSound::Artist, QString("event%1").arg(m_lastTraceItem->eventItem()->eventId()));
        snd.setProperty(CSound::Title, QString("%1.%2.%3.%4")
                .arg(m_lastTraceItem->fdsn())
                .arg(m_lastTraceItem->station())
                .arg(m_lastTraceItem->location().trimmed())
                .arg(m_lastTraceItem->channel()));
        snd.setProperty(CSound::Comment, m_reply->request().url().toString());

        // 3: unpack the samples and write it to the sound object
        int currentFrame = -1;
        qint32 tmpFrames[10240];
        float *soundFrames = snd.frames();
        foreach (QtMiniSEEDRecord *rec, recordList)
        {
            rec->unpack(&tmpFrames, 0, rec->sampleCount());
            for (int i = 0; i < rec->sampleCount(); ++i)
            {
                soundFrames[++currentFrame] = tmpFrames[i];
            }
        }

        // 4: calculate the DC offset
        float DC = 0.0;
        for (int i = 0; i < totalFrameCount; ++i)
        {
            DC += soundFrames[i] / totalFrameCount;
        }

        // 5: apply DC offset and calculate maximum and minimum
        float max, min;

        soundFrames[0] -= DC;
        min = max = soundFrames[0];
        for (int i = 1; i < totalFrameCount; ++i)
        {
            soundFrames[i] -= DC;

            if (min > soundFrames[i])
            {
                min = soundFrames[i];
            }
            else if (max < soundFrames[i])
            {
                max = soundFrames[i];
            }
        }

        // 6: normalize the sound
        float absMax = 1.0 / qMax(qAbs(min), qAbs(max));
        for (int i = 0; i < totalFrameCount; ++i)
        {
            soundFrames[i] *= absMax;
        }

        emit soundCreated(snd);
    }

    qDeleteAll(recordList);
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
