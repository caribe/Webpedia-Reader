#-------------------------------------------------
#
# Project created by QtCreator 2011-05-10T08:00:21
#
#-------------------------------------------------

QT       += core gui webkit network xml

TARGET = WebpediaFeeds
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connection.cpp \
    data/source.cpp \
    data/post.cpp \
    model/modelsources.cpp \
    model/modelposts.cpp \
    posts/postslist.cpp \
    posts/postsframe.cpp \
    sources/sourceframe.cpp \
    dialogs/login.cpp \
    browser/browserframe.cpp \
    dialogs/options.cpp \
    dialogs/wizardurl.cpp \
    dialogs/wizardlist.cpp

HEADERS  += mainwindow.h \
    connection.h \
    data/post.h \
    data/source.h \
    model/modelsources.h \
    model/modelposts.h \
    posts/postslist.h \
    posts/postsframe.h \
    sources/sourceframe.h \
    dialogs/login.h \
    browser/browserframe.h \
    dialogs/options.h \
    dialogs/wizardurl.h \
    dialogs/wizardlist.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources/webpedia.png \
    resources/user.png \
    resources/star.png \
    resources/star-empty.png \
    resources/plug-disconnect.png \
    resources/plug-connect.png \
    resources/pencil.png \
    resources/navigation-180-button.png \
    resources/mail.png \
    resources/mail-open.png \
    resources/home.png \
    resources/folder-open-feed.png \
    resources/folder--plus.png \
    resources/flag.png \
    resources/flag-white.png \
    resources/feed.png \
    resources/feed-plus.png \
    resources/cross.png \
    resources/cross-button.png \
    resources/clipboard-paste.png \
    resources/arrow.png \
    resources/arrow-circle.png \
    resources/arrow-180.png \
    resources/eye.png \
    resources/network-status.png \
    resources/network-status-busy.png \
    resources/wrench.png \
    resources/webpedia.svg \
    resources/magnifier.png \
    resources/heart.png \
    resources/clock.png \
    resources/plus.png \
    resources/application-browser.png \
    resources/funnel.png
