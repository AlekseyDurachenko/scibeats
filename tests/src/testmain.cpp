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
#include <QApplication>
#include "test_sounddata.h"
#include "test_writablesound.h"
#include "test_sound.h"
#include "test_soundtags.h"
#include "test_soundwriterrequest.h"
#include "test_soundwriterreply.h"
#include "test_soundwriter.h"
#include "test_soundpeaksdata.h"
#include "test_soundpeaks.h"
#include "test_writablesoundpeaks.h"
#include "test_soundpeaksrequest.h"
#include "test_soundpeaksreply.h"
//#include "testsoundpeakscalculator.h"
#include "test_soundproviderrequest.h"
#include "test_soundproviderreply.h"
#include "test_logmessage.h"
#include "test_plugin_soundprovider_factory.h"
#include "plugin_soundprovider_factory.h"
#include "test_plugin_soundprovider_fdsnws_parameters.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTEST_DISABLE_KEYPAD_NAVIGATION;

    plugin::soundprovider::Factory::init();

    int retval = QTest::qExec(new Test_LogMessage, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundData, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_WritableSound, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_Sound, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundTags, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundWriterRequest, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundWriterReply, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundWriter, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundPeaksData, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundPeaks, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_WritableSoundPeaks, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundPeaksRequest, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundPeaksReply, argc, argv);
    if (retval) {
        return retval;
    }

//    retval = QTest::qExec(new TestSoundPeaksCalculator, argc, argv);
//    if (retval) {
//        return retval;
//    }

    retval = QTest::qExec(new Test_SoundProviderRequest, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_SoundProviderReply, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_Plugin_SoundProvider_Factory, argc, argv);
    if (retval) {
        return retval;
    }

    retval = QTest::qExec(new Test_Plugin_SoundProvider_Fdsnws_Parameters, argc, argv);
    if (retval) {
        return retval;
    }

    return 0;
}
