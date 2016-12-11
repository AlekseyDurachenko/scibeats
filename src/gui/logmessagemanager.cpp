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
#include "logmessagemanager.h"


LogMessageManager::LogMessageManager(QObject *parent)
    : QObject(parent)
{
}

void LogMessageManager::append(const LogMessage &data)
{
    const int pos = m_items.count();
    emit aboutToBeInserted(pos, pos);
    m_items.append(data);
    emit inserted(pos, pos);
}

void LogMessageManager::remove(int first, int last)
{
    if (last == -1) {
        last = first;
    }

    if (first > last) {
        qSwap(first, last);
    }

    emit aboutToBeRemoved(first, last);
    for (int i = first; i <= last; ++i) {
        m_items.removeAt(i);
    }
    emit removed(first, last);
}

void LogMessageManager::removeAll()
{
    const int count = m_items.count();
    if (count == 0) {
        return;
    }

    emit aboutToBeRemoved(0, count - 1);
    m_items.clear();
    emit removed(0, count - 1);
}
