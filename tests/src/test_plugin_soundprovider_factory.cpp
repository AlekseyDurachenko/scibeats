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
#include "test_plugin_soundprovider_factory.h"
#include "plugin_soundprovider_factory.h"


static QByteArray fromRc(const QString &path)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    return file.readAll();
}


Test_Plugin_SoundProvider_Factory::Test_Plugin_SoundProvider_Factory(QObject *parent)
    : QObject(parent)
{
}

void Test_Plugin_SoundProvider_Factory::test_findId()
{
    const int testCaseCount = 4;
    struct TestCase {
        QString rcName;
        QByteArray res;
    } testCases[testCaseCount] = {
        ":/tests/test_plugin_soundprovider_factory_findid_test_1.xml",
        "fdsnws",

        ":/tests/test_plugin_soundprovider_factory_findid_test_2.xml",
        "fdsnws",

        ":/tests/test_plugin_soundprovider_factory_findid_test_3.xml",
        "none",

        ":/tests/test_plugin_soundprovider_factory_findid_test_4.xml",
        ""
    };

    // test each case
    for (int i = 0; i < testCaseCount; ++i) {
        SoundProviderRequest req;
        req.setQuery(fromRc(testCases[i].rcName));
        qDebug() << "CASE: " << testCases[i].rcName;
        QCOMPARE(plugin::soundprovider::Factory::findId(req), testCases[i].res);
    }
}

void Test_Plugin_SoundProvider_Factory::test_validateRequest()
{
    const int testCaseCount = 3;
    struct TestCase {
        QString rcName;
        bool res;
    } testCases[testCaseCount] = {
        ":/tests/test_plugin_soundprovider_factory_findid_test_3.xml",
        false,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_1.xml",
        true,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_3.xml",
        false
    };

    // test each case
    for (int i = 0; i < testCaseCount; ++i) {
        SoundProviderRequest req;
        req.setQuery(fromRc(testCases[i].rcName));
        qDebug() << "CASE: " << testCases[i].rcName;
        QCOMPARE(plugin::soundprovider::Factory::validateRequest(req), testCases[i].res);
    }
}

void Test_Plugin_SoundProvider_Factory::test_tagsFromRequest()
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

    const int testCaseCount = 3;
    struct TestCase {
        QString rcName;
        SoundTags res;
    } testCases[testCaseCount] = {
        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_1.xml",
        tags,

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_2.xml",
        SoundTags(),

        ":/tests/test_plugin_soundprovider_fdsnws_parameters_test_6.xml",
        SoundTags()
    };

    // test each case
    for (int i = 0; i < testCaseCount; ++i) {
        SoundProviderRequest req;
        req.setQuery(fromRc(testCases[i].rcName));
        qDebug() << "CASE: " << testCases[i].rcName;
        QCOMPARE(plugin::soundprovider::Factory::tagsFromRequest(req), testCases[i].res);
    }
}
