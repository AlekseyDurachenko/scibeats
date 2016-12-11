INCLUDEPATH    +=                                                   \
    $$PWD/src                                                       \
    $$PWD/src/datatype                                              \
    $$PWD/src/soundwriter                                           \
    $$PWD/src/soundpeaks                                            \
    $$PWD/src/soundobject                                           \
    $$PWD/src/gui                                                   \
    $$PWD/src/gui/soundwidget                                       \
    $$PWD/src/gui/soundwidget/soundcontrolwidget                    \
    $$PWD/src/gui/soundwidget/soundscrollwidget                     \
    $$PWD/src/soundprovider                                         \
    $$PWD/src/plugins/soundprovider                                 \
    $$PWD/src/plugins/soundprovider/fdsnws                          \


HEADERS        +=                                                   \
    $$PWD/src/consts.h                                              \
    $$PWD/src/theme.h                                               \
    $$PWD/src/version.h                                             \
    $$PWD/src/resources.h                                           \
    $$PWD/src/datatype/sounddata.h                                  \
    $$PWD/src/datatype/sound.h                                      \
    $$PWD/src/datatype/writablesound.h                              \
    $$PWD/src/datatype/soundtags.h                                  \
    $$PWD/src/soundwriter/soundwriter.h                             \
    $$PWD/src/soundwriter/soundwriterworker.h                       \
    $$PWD/src/soundwriter/soundwriterreply.h \
    $$PWD/src/soundwriter/soundwriterrequest.h \
    $$PWD/src/soundwriter/soundwriterresultstate.h \
    $$PWD/src/soundwriter/soundfileformat.h \
    $$PWD/src/datatype/soundpeaksdata.h \
    $$PWD/src/datatype/soundpeaks.h \
    $$PWD/src/datatype/writablesoundpeaks.h \
    $$PWD/src/soundpeaks/soundpeaksrequest.h \
    $$PWD/src/soundpeaks/soundpeaksresultstate.h \
    $$PWD/src/soundpeaks/soundpeaksworker.h \
    $$PWD/src/soundpeaks/soundpeaksreply.h \
    $$PWD/src/soundpeaks/soundpeakscalculator.h \
    $$PWD/src/soundobject/soundobject.h \
    $$PWD/src/gui/mainwindow.h \
    $$PWD/src/soundprovider/soundproviderrequest.h \
    $$PWD/src/soundprovider/soundproviderreply.h \
    $$PWD/src/soundprovider/soundproviderresultstate.h \
    $$PWD/src/soundprovider/abstractsoundproviderworker.h  \
    $$PWD/src/soundprovider/soundprovider.h  \
    $$PWD/src/datatype/logmessage.h \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider_factory.h \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider_interface.h \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider_abstractwidget.h \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_interface.h \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_widget.h \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_worker.h \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_parameters.h \
    $$PWD/src/gui/soundwindow.h \
    $$PWD/src/gui/soundrequestlogwidget.h \
    $$PWD/src/gui/logmessagemanager.h \
    $$PWD/src/gui/logmessageitemmodel.h \
    $$PWD/src/gui/soundwidget/soundwidget.h \
    $$PWD/src/gui/soundwidget/soundscrollwidget.h \
    $$PWD/src/gui/soundwidget/soundcontrolselectionwidget.h \
    $$PWD/src/gui/soundwidget/soundcontrolpanelwidget.h \
    $$PWD/src/gui/soundwidget/soundcontrolsampleratewidget.h \
    $$PWD/src/gui/soundwidget/soundcontrolgeometrywidget.h \
    $$PWD/src/gui/soundwidget/soundcontrolbuttonboxwidget.h \
    $$PWD/src/gui/soundwidget/sounddatasource.h

SOURCES        +=                                                   \
    $$PWD/src/theme.cpp                                             \
    $$PWD/src/version.cpp                                           \
    $$PWD/src/resources.cpp                                         \
    $$PWD/src/datatype/sounddata.cpp                                \
    $$PWD/src/datatype/sound.cpp                                    \
    $$PWD/src/datatype/writablesound.cpp                            \
    $$PWD/src/datatype/soundtags.cpp                                \
    $$PWD/src/soundwriter/soundwriter.cpp                           \
    $$PWD/src/soundwriter/soundwriterworker.cpp                     \
    $$PWD/src/soundwriter/soundwriterreply.cpp \
    $$PWD/src/soundwriter/soundwriterrequest.cpp \
    $$PWD/src/soundwriter/soundfileformat.cpp \
    $$PWD/src/datatype/soundpeaksdata.cpp \
    $$PWD/src/datatype/soundpeaks.cpp \
    $$PWD/src/datatype/writablesoundpeaks.cpp \
    $$PWD/src/soundpeaks/soundpeaksrequest.cpp \
    $$PWD/src/soundpeaks/soundpeaksworker.cpp \
    $$PWD/src/soundpeaks/soundpeaksreply.cpp \
    $$PWD/src/soundpeaks/soundpeakscalculator.cpp \
    $$PWD/src/soundobject/soundobject.cpp \
    $$PWD/src/gui/mainwindow.cpp \
    $$PWD/src/soundprovider/soundproviderrequest.cpp \
    $$PWD/src/soundprovider/soundproviderreply.cpp  \
    $$PWD/src/soundprovider/abstractsoundproviderworker.cpp  \
    $$PWD/src/soundprovider/soundprovider.cpp  \
    $$PWD/src/datatype/logmessage.cpp \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider_factory.cpp \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider_interface.cpp \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider_abstractwidget.cpp \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_interface.cpp \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_widget.cpp \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_worker.cpp \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws_parameters.cpp \
    $$PWD/src/gui/soundwindow.cpp \
    $$PWD/src/gui/soundrequestlogwidget.cpp \
    $$PWD/src/gui/logmessagemanager.cpp \
    $$PWD/src/gui/logmessageitemmodel.cpp \
    $$PWD/src/gui/soundwidget/soundwidget.cpp \
    $$PWD/src/gui/soundwidget/soundcontrolbuttonboxwidget.cpp \
    $$PWD/src/gui/soundwidget/soundcontrolgeometrywidget.cpp \
    $$PWD/src/gui/soundwidget/soundcontrolpanelwidget.cpp \
    $$PWD/src/gui/soundwidget/soundcontrolsampleratewidget.cpp \
    $$PWD/src/gui/soundwidget/soundcontrolselectionwidget.cpp \
    $$PWD/src/gui/soundwidget/soundscrollwidget.cpp \
    $$PWD/src/gui/soundwidget/sounddatasource.cpp


FORMS          +=                                                   \
    $$PWD/src/gui/mainwindow.ui \
    $$PWD/src/gui/soundrequestlogwidget.ui \
    $$PWD/src/gui/soundwidget.ui


TRANSLATIONS   +=                                                   \
    $$PWD/translations/ru.ts                                        \


RESOURCES      +=                                                   \
    $$PWD/src/plugins/soundprovider/plugin_soundprovider.qrc        \
    $$PWD/src/plugins/soundprovider/fdsnws/plugin_soundprovider_fdsnws.qrc  \


OTHER_FILES    +=                                                   \
    $$PWD/ABOUT                                                     \
    $$PWD/AUTHORS                                                   \
    $$PWD/CHANGELOG                                                 \
    $$PWD/LIBRARIES                                                 \
    $$PWD/LICENSE                                                   \
    $$PWD/LICENSE.GPL-3+                                            \
    $$PWD/README.md                                                 \
    $$PWD/translations/docs/ABOUT_ru                                \
    $$PWD/translations/docs/AUTHORS_ru                              \
    $$PWD/translations/docs/CHANGELOG_ru                            \
    $$PWD/translations/docs/LIBRARIES_ru                            \


!contains(QT, testlib) {
    HEADERS   +=                                                    \

    SOURCES   +=                                                    \
        $$PWD/src/main.cpp                                          \

}

win32 {
    RC_FILE     += res/windows/rc.rc
}
