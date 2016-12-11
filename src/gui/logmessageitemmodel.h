// Copyright 2016, Durachenko Aleksey V. <durachenko@gs.sbras.ru>
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
#ifndef LOGMESSAGEITEMMODEL_H
#define LOGMESSAGEITEMMODEL_H


#include "logmessagemanager.h"
#include <QAbstractItemModel>


class LogMessageItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LogMessageItemModel(LogMessageManager *mgr, QObject *parent = 0);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

private slots:
    void mgr_aboutToBeInserted(int first, int last);
    void mgr_inserted(int first, int last);
    void mgr_aboutToBeRemoved(int first, int last);
    void mgr_removed(int first, int last);
    void mgr_destroyed();

private:
    LogMessageManager *m_mgr;
};


#endif // LOGMESSAGEITEMMODEL_H
