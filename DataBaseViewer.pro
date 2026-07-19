QT += widgets sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/src

SOURCES += \
    main.cpp \
    src/core/connectioninfo.cpp \
    src/core/databaseconnection.cpp \
    src/widgets/dialogConnectionSettings.cpp \
    src/widgets/mainwindow.cpp

HEADERS += \
    src/core/connectioninfo.h \
    src/core/databaseconnection.h \
    src/widgets/dialogConnectionSettings.h \
    src/widgets/mainwindow.h

FORMS += \
    src/widgets/dialogConnectionSettings.ui \
    src/widgets/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
