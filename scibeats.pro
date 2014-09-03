# Copyright 2014, Durachenko Aleksey V. <durachenko.aleksey@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
TARGET          = scibeats
TEMPLATE        = app

CONFIG         += console debug_and_release
QT             += core gui network xml

DESTDIR         = bin
OBJECTS_DIR     = build/release_obj
MOC_DIR         = build/release_moc
UI_DIR          = build/release_ui
RCC_DIR         = build/release_rcc

include($$PWD/libs/qtsampleengine/qtsampleengine.pri)
include($$PWD/libs/qtminiseed/qtminiseed.pri)
include($$PWD/libs/libmseed/libmseed.pri)

DEFINES        +=                                           \

INCLUDEPATH    +=                                           \

INCLUDEPATH    +=                                           \
    src                                                     \
    src/soundwidget                                         \
    src/datatypes                                           \
    src/plugins/datasource                                  \
    src/plugins/playbackend                                 \

HEADERS        +=                                           \
    src/consts.h                                            \
    src/global.h                                            \
    src/cmainwindow.h                                       \
    src/cdatasourcewidget.h                                 \
    src/csoundpropertywidget.h                              \
    src/cexportdialog.h                                     \
    src/csndfile.h                                          \
    src/caboutdialog.h                                      \
    src/datatypes/csound.h                                  \
    src/datatypes/csounddata.h                              \
    src/soundwidget/csoundplotcontroller.h                  \
    src/soundwidget/csoundscrollbarwidget.h                 \
    src/soundwidget/csoundpeakreader.h                      \
    src/soundwidget/csoundplotwidget.h                      \
    src/soundwidget/csoundwidget.h                          \
    src/plugins/datasource/plugin_datasource_interface.h        \
    src/plugins/datasource/plugin_datasource_factory.h          \
    src/plugins/datasource/plugin_datasource_cabstractwidget.h  \
    src/plugins/datasource/iris/plugin_datasource_iris.h                    \
    src/plugins/datasource/iris/plugin_datasource_iris_ccontrolwidget.h     \
    src/plugins/datasource/iris/plugin_datasource_iris_citemmodel.h         \
    src/plugins/datasource/iris/plugin_datasource_iris_utils.h              \
    src/plugins/datasource/iris/plugin_datasource_iris_cabstractitem.h      \
    src/plugins/datasource/iris/plugin_datasource_iris_ctraceitem.h         \
    src/plugins/datasource/iris/plugin_datasource_iris_cqueryitem.h         \
    src/plugins/datasource/iris/plugin_datasource_iris_cprogressitem.h      \
    src/plugins/datasource/iris/plugin_datasource_iris_ceventitem.h         \
    src/plugins/datasource/iris/plugin_datasource_iris_cnotfounditem.h      \
    src/plugins/datasource/iris/plugin_datasource_iris_consts.h             \
    src/plugins/playbackend/plugin_playbackend_cabstractplaybackend.h   \
    src/plugins/playbackend/plugin_playbackend_cabstractconfigwidget.h  \
    src/plugins/playbackend/plugin_playbackend_factory.h                \
    src/plugins/playbackend/plugin_playbackend_interface.h              \
    src/plugins/playbackend/portaudio/plugin_playbackend_portaudio_cconfigwidget.h  \
    src/plugins/playbackend/portaudio/plugin_playbackend_portaudio_cplaybackend.h   \
    src/plugins/playbackend/portaudio/plugin_playbackend_portaudio.h                \

SOURCES        +=                                           \
    src/cmainwindow.cpp                                     \
    src/cdatasourcewidget.cpp                               \
    src/csoundpropertywidget.cpp                            \
    src/cexportdialog.cpp                                   \
    src/csndfile.cpp                                        \
    src/caboutdialog.cpp                                    \
    src/datatypes/csound.cpp                                \
    src/datatypes/csounddata.cpp                            \
    src/soundwidget/csoundplotcontroller.cpp                \
    src/soundwidget/csoundscrollbarwidget.cpp               \
    src/soundwidget/csoundpeakreader.cpp                    \
    src/soundwidget/csoundplotwidget.cpp                    \
    src/soundwidget/csoundwidget.cpp                        \
    src/plugins/datasource/plugin_datasource_interface.cpp      \
    src/plugins/datasource/plugin_datasource_factory.cpp        \
    src/plugins/datasource/plugin_datasource_cabstractwidget.cpp\
    src/plugins/datasource/iris/plugin_datasource_iris.cpp                  \
    src/plugins/datasource/iris/plugin_datasource_iris_ccontrolwidget.cpp   \
    src/plugins/datasource/iris/plugin_datasource_iris_citemmodel.cpp       \
    src/plugins/datasource/iris/plugin_datasource_iris_utils.cpp            \
    src/plugins/datasource/iris/plugin_datasource_iris_cabstractitem.cpp    \
    src/plugins/datasource/iris/plugin_datasource_iris_ctraceitem.cpp       \
    src/plugins/datasource/iris/plugin_datasource_iris_cqueryitem.cpp       \
    src/plugins/datasource/iris/plugin_datasource_iris_cprogressitem.cpp    \
    src/plugins/datasource/iris/plugin_datasource_iris_ceventitem.cpp       \
    src/plugins/datasource/iris/plugin_datasource_iris_cnotfounditem.cpp    \
    src/plugins/playbackend/plugin_playbackend_cabstractplaybackend.cpp \
    src/plugins/playbackend/plugin_playbackend_cabstractconfigwidget.cpp\
    src/plugins/playbackend/plugin_playbackend_factory.cpp              \
    src/plugins/playbackend/plugin_playbackend_interface.cpp            \
    src/plugins/playbackend/portaudio/plugin_playbackend_portaudio_cconfigwidget.cpp\
    src/plugins/playbackend/portaudio/plugin_playbackend_portaudio_cplaybackend.cpp \
    src/plugins/playbackend/portaudio/plugin_playbackend_portaudio.cpp              \

FORMS          +=                                           \
    src/cmainwindow.ui                                      \
    src/cdatasourcewidget.ui                                \
    src/csoundpropertywidget.ui                             \
    src/cexportdialog.ui                                    \
    src/caboutdialog.ui                                     \
    src/plugins/datasource/iris/plugin_datasource_iris_ccontrolwidget.ui    \


RESOURCES      +=                                           \
    qrc/icons.qrc                                           \
    qrc/about.qrc                                           \
    src/plugins/datasource/iris/plugin_datasource_iris.qrc  \

OTHER_FILES    +=                                           \
    AUTHORS                                                 \
    CHANGELOG                                               \
    LICENSE                                                 \

# if you want to test the project, do: qmake "QT += testlib"
contains(QT, testlib) {
    SOURCES   +=                                            \

    HEADERS   +=                                            \

} else {
    SOURCES   +=                                            \
        src/main.cpp                                        \

}

win32 {
    CONFIG += embed_manifest_exe
    RC_FILE = scibeats.rc
}

# debug
build_pass:CONFIG(debug, debug|release) {
    TARGET      = $$join(TARGET,,,d)
    OBJECTS_DIR = $$join(OBJECTS_DIR,,,d)
    MOC_DIR     = $$join(MOC_DIR,,,d)
    UI_DIR      = $$join(UI_DIR,,,d)
    RCC_DIR     = $$join(RCC_DIR,,,d)

    win32 {
    }

    unix {
        LIBS += -lportaudio -lsamplerate -lsndfile
        DEFINES += "G_VERSION=\"\\\"$$system($$PWD/get_version.sh)\\\"\""
    }
}

# release
build_pass:CONFIG(release, debug|release) {
    win32 {
        CONFIG -= console 
        LIBS += -lportaudio -lsamplerate -lsndfile-1
        DEFINES += "G_VERSION=\"\\\"$$system('sh -c $$PWD/get_version.sh')\\\"\""
    }

    unix {
        LIBS += -lportaudio -lsamplerate -lsndfile
        DEFINES += "G_VERSION=\"\\\"$$system($$PWD/get_version.sh)\\\"\""
    }
}
