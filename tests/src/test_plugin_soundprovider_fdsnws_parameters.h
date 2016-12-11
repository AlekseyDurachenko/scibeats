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
#ifndef TEST_PLUGIN_SOUNDPROVIDER_FDSNWS_PARAMETERS_H
#define TEST_PLUGIN_SOUNDPROVIDER_FDSNWS_PARAMETERS_H


#include <QObject>
#include <QtTest/QtTest>


class Test_Plugin_SoundProvider_Fdsnws_Parameters : public QObject
{
    Q_OBJECT
public:
    explicit Test_Plugin_SoundProvider_Fdsnws_Parameters(QObject *parent = 0);

private slots:
    void test_setGet();
    void test_buildQueryUrl();
    void test_validate();
    void test_fromXml();
    void test_toXml();
};


#endif // TEST_PLUGIN_SOUNDPROVIDER_FDSNWS_PARAMETERS_H
