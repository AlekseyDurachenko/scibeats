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
#ifndef LOGMESSAGEMANAGER_H
#define LOGMESSAGEMANAGER_H


#include "logmessage.h"
#include <QObject>


class LogMessageManager : public QObject
{
    Q_OBJECT
public:
    explicit LogMessageManager(QObject *parent = 0);

    inline int count() const;
    inline const LogMessage &at(int i) const;

public slots:
    void append(const LogMessage &data);
    void remove(int first, int last = -1);
    void removeAll();

signals:
    void aboutToBeInserted(int first, int last);
    void inserted(int first, int last);
    void aboutToBeRemoved(int first, int last);
    void removed(int first, int last);

private:
    QList<LogMessage> m_items;
};

int LogMessageManager::count() const
{
    return m_items.count();
}

const LogMessage &LogMessageManager::at(int i) const
{
    return m_items.at(i);
}


#endif // LOGMESSAGEMANAGER_H
