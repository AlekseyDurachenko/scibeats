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
#include "test_logmessage.h"
#include "logmessage.h"
#include <QRegExp>


Test_LogMessage::Test_LogMessage(QObject *parent)
    : QObject(parent)
{
}

void Test_LogMessage::test_defaultConstructor()
{
    const LogMessage logMsg;
    QCOMPARE(logMsg.type(), LogMessage::Information);
    QCOMPARE(logMsg.message(), QString());
    QCOMPARE(logMsg.dateTime().date().year(), QDateTime::currentDateTime().date().year());
}

void Test_LogMessage::test_constructor()
{
    const LogMessage logMsg(LogMessage::Critical, QString("error"));
    QCOMPARE(logMsg.type(), LogMessage::Critical);
    QCOMPARE(logMsg.message(), QString("error"));
    QCOMPARE(logMsg.dateTime().date().year(), QDateTime::currentDateTime().date().year());
}

void Test_LogMessage::test_toString()
{
    const LogMessage logMsgInfo(LogMessage::Information, QString("text"));
    const LogMessage logMsgWarn(LogMessage::Warning, QString("text"));
    const LogMessage logMsgCrit(LogMessage::Critical, QString("text"));
    const QRegExp regExpInfo("^\\[INFO\\]\\([0-9\\-\\ \\:]*\\)\\ text");
    const QRegExp regExpWarn("^\\[WARN\\]\\([0-9\\-\\ \\:]*\\)\\ text");
    const QRegExp regExpCrit("^\\[CRIT\\]\\([0-9\\-\\ \\:]*\\)\\ text");
    QVERIFY((regExpInfo.exactMatch(logMsgInfo.toString())));
    QVERIFY((regExpWarn.exactMatch(logMsgWarn.toString())));
    QVERIFY((regExpCrit.exactMatch(logMsgCrit.toString())));
}
