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
#include "test_soundproviderrequest.h"
#include "soundproviderrequest.h"


Test_SoundProviderRequest::Test_SoundProviderRequest(QObject *parent)
    : QObject(parent)
{
}

void Test_SoundProviderRequest::test_setGet()
{
    SoundProviderRequest request;
    QCOMPARE(request.isNull(), true);

    request.setQuery(QByteArray("abcdef"));
    QCOMPARE(request.isNull(), false);
    QCOMPARE(request.query(), QByteArray("abcdef"));
}

void Test_SoundProviderRequest::test_eq()
{
    SoundProviderRequest req1;
    SoundProviderRequest req2;
    SoundProviderRequest req3;
    req1.setQuery(QByteArray("req1"));
    req2.setQuery(QByteArray("req2"));
    req3.setQuery(QByteArray("req1"));
    QVERIFY(req1 == req1);
    QVERIFY(req1 != req2);
    QVERIFY(req1 == req3);
}
