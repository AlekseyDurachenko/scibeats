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
#include "plugin_datasource_iris_cprogressitem.h"
#include <QTimer>
#include <QIcon>

// ----------------------------------------------------------------------
namespace plugin {
namespace datasource {
namespace iris {
// ----------------------------------------------------------------------

CProgressItem::CProgressItem(int row, QObject *parent) :
    CAbstractItem(Progress, row, parent)
{
    m_step = 1;
    m_bytesTotal = 0;
    m_bytesReceived = 0;

    m_timer = new QTimer(this);
    m_timer->start(150);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
}

QVariant CProgressItem::data(int column, int role) const
{
    if (role == Qt::DisplayRole && column == 0)
    {
        return tr("%1 of %2 is received").arg(bytesToString(m_bytesReceived, true), bytesToString(m_bytesTotal));
    }
    else if (role == Qt::DecorationRole && column == 0)
    {
        return QIcon(QString(":/plugin/datasource/iris/icons/progress-loading-%1.png").arg(m_step));
    }

    return QVariant();
}

void CProgressItem::setProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_bytesReceived = bytesReceived;
    m_bytesTotal = bytesTotal;
}

void CProgressItem::slot_timeout()
{
    if (++m_step > 8)
    {
        m_step = 1;
    }

    emit dataChanged(qobject_cast<CAbstractItem *>(parent()), row(), row());
}

QString CProgressItem::bytesToString(qint64 bytes, bool show_zero) const
{
    if (bytes >= 1024*1024)
    {
        return tr("%1 mibibytes").arg(bytes/1024.0/1024.0, 0, 'f', 3);
    }
    else if (bytes >= 1024)
    {
        return tr("%1 kibibytes").arg(bytes/1024);
    }
    else if (bytes > 0 || show_zero)
    {
        return tr("%1 bytes").arg(bytes);
    }

    return "???";
}

// ----------------------------------------------------------------------
} // namespace iris
} // namespace datasource
} // namespace plugin
// ----------------------------------------------------------------------
