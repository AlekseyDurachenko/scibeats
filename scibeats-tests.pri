contains(QT, testlib) {
    SOURCES   +=                                        \
        $$PWD/tests/src/test_plugin_soundprovider_factory.cpp               \
        $$PWD/tests/src/test_plugin_soundprovider_fdsnws_parameters.cpp     \
        $$PWD/tests/src/test_logmessage.cpp             \
        $$PWD/tests/src/test_sound.cpp                  \
        $$PWD/tests/src/testmain.cpp                    \
        $$PWD/tests/src/test_sounddata.cpp              \
        $$PWD/tests/src/test_soundpeaks.cpp             \
        $$PWD/tests/src/test_soundpeaksdata.cpp         \
        $$PWD/tests/src/test_soundpeaksreply.cpp        \
        $$PWD/tests/src/test_soundpeaksrequest.cpp      \
        $$PWD/tests/src/test_soundproviderreply.cpp     \
        $$PWD/tests/src/test_soundproviderrequest.cpp   \
        $$PWD/tests/src/test_soundtags.cpp              \
        $$PWD/tests/src/test_soundwriter.cpp            \
        $$PWD/tests/src/test_soundwriterreply.cpp       \
        $$PWD/tests/src/test_soundwriterrequest.cpp     \
        $$PWD/tests/src/test_writablesound.cpp          \
        $$PWD/tests/src/test_writablesoundpeaks.cpp     \


    HEADERS   +=                                        \
        $$PWD/tests/src/test_plugin_soundprovider_factory.h                 \
        $$PWD/tests/src/test_plugin_soundprovider_fdsnws_parameters.h       \
        $$PWD/tests/src/test_logmessage.h               \
        $$PWD/tests/src/test_sound.h                    \
        $$PWD/tests/src/test_sounddata.h                \
        $$PWD/tests/src/test_soundpeaks.h               \
        $$PWD/tests/src/test_soundpeaksdata.h           \
        $$PWD/tests/src/test_soundpeaksreply.h          \
        $$PWD/tests/src/test_soundpeaksrequest.h        \
        $$PWD/tests/src/test_soundproviderreply.h       \
        $$PWD/tests/src/test_soundproviderrequest.h     \
        $$PWD/tests/src/test_soundtags.h                \
        $$PWD/tests/src/test_soundwriter.h              \
        $$PWD/tests/src/test_soundwriterreply.h         \
        $$PWD/tests/src/test_soundwriterrequest.h       \
        $$PWD/tests/src/test_writablesound.h            \
        $$PWD/tests/src/test_writablesoundpeaks.h       \




}

RESOURCES      +=                                       \
        $$PWD/tests/src/test_plugin_soundprovider.qrc   \
        $$PWD/tests/src/test_plugin_soundprovider_fdsnws.qrc                \
