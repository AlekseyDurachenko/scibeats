win32 {
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


    INSTALLS += target docs translations icons

    target.path = /
    docs.path   = /
    icons.path  = /icons
    translations.path = /translations
}
