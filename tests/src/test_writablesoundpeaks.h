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
#ifndef TEST_WRITABLESOUNDPEAKS_H
#define TEST_WRITABLESOUNDPEAKS_H


#include <QObject>
#include <QtTest/QtTest>


class Test_WritableSoundPeaks : public QObject
{
    Q_OBJECT
public:
    explicit Test_WritableSoundPeaks(QObject *parent = 0);

private slots:
    void test_defaultConstructor();
    void test_createAndClear();
    void test_create_001();
    void test_create_002();
    void test_create_003();
    void test_create_004();
    void test_samples_001();
    void test_samples_002();
    void test_samples_003();
    void test_samples_004();
};


#endif // TEST_WRITABLESOUNDPEAKS_H
