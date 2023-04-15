QT       += core gui
QT += network
_COMPAT_LAYER=RUNASINVOKER
RC_ICONS = icone.ico
QMAKE_LFLAGS += -static
CONFIG += c++17
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
VERSION = 1.7
QMAKE_TARGET_COPYRIGHT = Julien LAPORTE
QMAKE_TARGET_DESCRIPTION = "Outil d'aide a la sauvegarde et la restauration d'un poste windows"
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../../Users/Sam/Documents/configOffisave/ConfigMainWindow.cpp \
    ../../Users/Sam/Documents/configOffisave/ListeRepertoiresDepuis.cpp \
    ../../Users/Sam/Documents/configOffisave/ManageConfigFile.cpp \
    static/ConfigFile.cpp \
    static/ConfigWindow.cpp \
    static/Controller.cpp \
    static/CopyProgress.cpp \
    static/Executer.cpp \
    static/InventaireFichiers.cpp \
    static/InventaireRepertoire.cpp \
    static/Loading.cpp \
    static/LocationWindow.cpp \
    static/Maintenance.cpp \
    static/NetUse.cpp \
    static/functions.cpp \
    static/MainWindow.cpp \
    static/QRes.cpp \
    static/Autosave.cpp \
    static/CipherByteArray.cpp \
    main.cpp \
    static/PlanificationWindow.cpp

HEADERS += \
    ../../Users/Sam/Documents/configOffisave/ConfigMainWindow.h \
    ../../Users/Sam/Documents/configOffisave/ListeRepertoiresDepuis.h \
    ../../Users/Sam/Documents/configOffisave/ManageConfigFile.h \
    static/ConfigFile.h \
    static/ConfigWindow.h \
    static/Controller.h \
    static/CopyProgress.h \
    static/Executer.h \
    static/InventaireFichiers.h \
    static/InventaireRepertoire.h \
    static/Loading.h \
    static/LocationWindow.h \
    static/MainWindow.h \
    static/Maintenance.h \
    static/NetUse.h \
    static/functions.h \
    static/QRes.h \
    static/Autosave.h \
    static/CipherByteArray.h \
    static/PlanificationWindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

FORMS += \
    ../../Users/Sam/Documents/configOffisave/ConfigMainWindow.ui \
    ../../Users/Sam/Documents/configOffisave/ListeRepertoiresDepuis.ui
