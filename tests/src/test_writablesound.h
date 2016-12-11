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
#ifndef TEST_WRITABLESOUND_H
#define TEST_WRITABLESOUND_H


#include <QObject>
#include <QtTest/QtTest>


class Test_WritableSound : public QObject
{
    Q_OBJECT
public:
    explicit Test_WritableSound(QObject *parent = 0);

private slots:
    void test_defaultConstructor();
    void test_createAndClear();
    void test_samples();
    void test_fromToSound();
};


#endif // TEST_WRITABLESOUND_H
