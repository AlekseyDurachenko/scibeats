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
#include "test_plugin_soundprovider_fdsnws_parameters.h"
#include "plugin_soundprovider_factory.h"
#include "plugin_soundprovider_fdsnws_parameters.h"


static QByteArray fromRc(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}


Test_Plugin_SoundProvider_Fdsnws_Parameters::Test_Plugin_SoundProvider_Fdsnws_Parameters(QObject *parent)
    : QObject(parent)
{
}

void Test_Plugin_SoundProvider_Fdsnws_Parameters::test_setGet()
{
    SoundTags tags;
    tags.setTitle("title");
    tags.setCopyright("copyright");
    tags.setSoftware("software");
    tags.setArtist("artist");
    tags.setComment("comment");
    tags.setDate("date");
    tags.setGenre("genre");
    tags.setTitle("title");

    plugin::soundprovider::fdsnws::Parameters params;
    params.setUrl(QUrl("http://localhost/"));
    params.setNetwork(QString("NE"));
    params.setStation(QString("STATI"));
    params.setLocation(QString("LO"));
    params.setChannel(QString("CHA"));
    params.setStartTime(QDateTime(QDate(2000, 1, 2), QTime(3, 4, 5), Qt::UTC));
    params.setEndTime(QDateTime(QDate(2006, 7, 8), QTime(9, 10, 11), Qt::UTC));
    params.setTags(tags);

    QCOMPARE(params.url(), QUrl("http://localhost/"));
    QCOMPARE(params.network(), QString("NE"));
    QCOMPARE(params.station(), QString("STATI"));
    QCOMPARE(params.location(), QString("LO"));
    QCOMPARE(params.channel(), QString("CHA"));
    QCOMPARE(params.startTime(), QDateTime(QDate(2000, 1, 2), QTime(3, 4, 5), Qt::UTC));
    QCOMPARE(params.endTime(), QDateTime(QDate(2006, 7, 8), QTime(9, 10, 11), Qt::UTC));
    QCOMPARE(params.tags(), tags);
}

void Test_Plugin_SoundProvider_Fdsnws_Parameters::test_buildQueryUrl()
{
    plugin::soundprovider::fdsnws::Parameters params;
    params.setUrl(QUrl("http://localhost.xyz/"));
    params.setNetwork(QString("NE"));
    params.setStation(QString("STATI"));
    params.setLocation(QString("LO"));
    params.setChannel(QString("CHA"));
    params.setStartTime(QDateTime(QDate(2000, 1, 2), QTime(3, 4, 5), Qt::UTC));
    params.setEndTime(QDateTime(QDate(2006, 7, 8), QTime(9, 10, 11), Qt::UTC));

    QCOMPARE(params.buildQueryUrl(),
             QUrl("http://localhost.xyz/?"
                  "net=NE&"
                  "sta=STATI&"
                  "loc=LO&"
                  "cha=CHA&"
                  "starttime=2000-01-02T03:04:05&"
                  "endtime=2006-07-08T09:10:11"));
}

void Test_Plugin_SoundProvider_Fdsnws_Parameters::test_validate()
{
    const int testCaseCount = 6;
    struct TestCase {
        QString rcName;
        bool result;
    } testCases[testCaseCount] = {
        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_1.xml",
        true,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_2.xml",
        false,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_3.xml",
        false,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_4.xml",
        false,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_5.xml",
        false,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_6.xml",
        true
    };

    // test each case
    for (int i = 0; i < testCaseCount; ++i) {
        qDebug() << "CASE: " << testCases[i].rcName;
        QCOMPARE(plugin::soundprovider::fdsnws::Parameters::validate(fromRc(testCases[i].rcName)), testCases[i].result);
    }
}

void Test_Plugin_SoundProvider_Fdsnws_Parameters::test_fromXml()
{
    SoundTags tags;
    tags.setTitle("title");
    tags.setCopyright("copyright");
    tags.setSoftware("software");
    tags.setArtist("artist");
    tags.setComment("comment");
    tags.setDate("date");
    tags.setGenre("genre");
    tags.setTitle("title");

    plugin::soundprovider::fdsnws::Parameters params;
    QCOMPARE(params.fromXml(fromRc(":/tests/test_plugin_soundprovider_fdsnws_parameters_test_1.xml")), true);
    QCOMPARE(params.url(), QUrl("https://service.iris.edu/fdsnws/dataselect/1/query"));
    QCOMPARE(params.network(), QString("IU"));
    QCOMPARE(params.station(), QString("ANMO"));
    QCOMPARE(params.location(), QString("00"));
    QCOMPARE(params.channel(), QString("BHZ"));
    QCOMPARE(params.startTime(), QDateTime(QDate(2010, 2, 27), QTime(6, 30, 0), Qt::UTC));
    QCOMPARE(params.endTime(), QDateTime(QDate(2010, 2, 27), QTime(10, 30, 0), Qt::UTC));
    QCOMPARE(params.tags(), tags);
}

void Test_Plugin_SoundProvider_Fdsnws_Parameters::test_toXml()
{
    SoundTags tags;
    tags.setTitle("title");
    tags.setCopyright("copyright");
    tags.setSoftware("software");
    tags.setArtist("artist");
    tags.setComment("comment");
    tags.setDate("date");
    tags.setGenre("genre");
    tags.setTitle("title");

    plugin::soundprovider::fdsnws::Parameters params;
    params.setUrl(QUrl("https://service.iris.edu/fdsnws/dataselect/1/query"));
    params.setNetwork(QString("IU"));
    params.setStation(QString("ANMO"));
    params.setLocation(QString("00"));
    params.setChannel(QString("BHZ"));
    params.setStartTime(QDateTime(QDate(2010, 2, 27), QTime(6, 30, 0), Qt::UTC));
    params.setEndTime(QDateTime(QDate(2010, 2, 27), QTime(10, 30, 0), Qt::UTC));
    params.setTags(tags);

    QCOMPARE(params.toXml(), fromRc(":/tests/test_plugin_soundprovider_fdsnws_parameters_test_1.xml"));
}
