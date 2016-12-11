unix {
    docs.files =                                                    \
        $$PWD/ABOUT                                                 \
        $$PWD/AUTHORS                                               \
        $$PWD/CHANGELOG                                             \
        $$PWD/LIBRARIES                                             \
        $$PWD/LICENSE                                               \
        $$PWD/LICENSE.GPL-3+                                        \
        $$PWD/translations/docs/*                                   \

    translations.files =                                            \
        $$PWD/translations/*.qm                                     \

    icons.files =                                                   \
        $$PWD/res/icons/hicolor/128x128/*                           \

    desktopfiles.files =                                            \
        $$PWD/res/desktopfiles/$${TARGET}.desktop                   \

    contains(DEFINES, APP_PORTABLE) {
        INSTALLS += target docs translations icons

        target.path = /
        docs.path   = /
        icons.path  = /icons
        translations.path = /translations
    }

    !contains(DEFINES, APP_PORTABLE) {
        INSTALLS += target docs translations icons desktopfiles    \
            appicon1024 appicon512 appicon128 appicon64 appicon32

        target.path         = /usr/bin/
        docs.path           = /usr/share/$${TARGET}/
        translations.path   = /usr/share/$${TARGET}/translations/
        icons.path          = /usr/share/$${TARGET}/icons/
        desktopfiles.path   = /usr/share/applications/

        appicon1024.files   = $$PWD/res/icons/hicolor/1024x1024/$${TARGET}.png
        appicon1024.path    = /usr/share/icons/hicolor/1024x1024/apps/

        appicon512.files    = $$PWD/res/icons/hicolor/512x512/$${TARGET}.png
        appicon512.path     = /usr/share/icons/hicolor/512x512/apps/

        appicon128.files    = $$PWD/res/icons/hicolor/128x128/$${TARGET}.png
        appicon128.path     = /usr/share/icons/hicolor/128x128/apps/

        appicon64.files     = $$PWD/res/icons/hicolor/64x64/$${TARGET}.png
        appicon64.path      = /usr/share/icons/hicolor/64x64/apps/

        appicon32.files     = $$PWD/res/icons/hicolor/32x32/$${TARGET}.png
        appicon32.path      = /usr/share/icons/hicolor/32x32/apps/

        # you should to determine the location of resources
        DEFINES += "APP_RESOURCES_PREFIX='/usr/share/$${TARGET}'"
    }
}
