// Copyright 2016, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
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
#include "logmessage.h"
#include <QObject>


static QString typeStr(LogMessage::Type type)
{
    switch (type) {
    case LogMessage::Information:
        return QObject::tr("INFO");
    case LogMessage::Warning:
        return QObject::tr("WARN");
    case LogMessage::Critical:
        return QObject::tr("CRIT");
    }

    return QString();
}


LogMessage::LogMessage(LogMessage::Type type, const QString &message)
{
    m_type = type;
    m_message = message;
    m_dateTime = QDateTime::currentDateTime();
}

QString LogMessage::toString() const
{
    return QString("[%1](%2) %3")
           .arg(typeStr(m_type))
           .arg(m_dateTime.toString("yyyy-MM-dd HH:mm:ss"))
           .arg(m_message);
}
