QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TCP-client/FileEncryption.cpp \
    TCP-client/FileHandeling.cpp \
    TCP-client/authenticationpopup.cpp \
    TCP-client/connectionPopup.cpp \
    TCP-client/tableHandler.cpp \
    TCP-client/tcpHandler.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    TCP-client/FileEncryption.h \
    TCP-client/FileHandeling.h \
    TCP-client/authenticationpopup.h \
    TCP-client/connectionPopup.h \
    TCP-client/tableHandler.h \
    TCP-client/tcpHandler.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    ../Qt-AES-1.2


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# will help enable depricated functions

DISTFILES +=

