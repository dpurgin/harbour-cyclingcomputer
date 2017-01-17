# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-cyclingcomputer

CONFIG += sailfishapp c++11

QT += positioning

SOURCES += \
    src/main.cpp \
    src/blesensor.cpp \
    src/cyclingcadencesensor.cpp \
    src/facilities.cpp \
    src/locationsensor.cpp \
    src/heartratemonitor.cpp

HEADERS += \
    src/blesensor.h \
    src/cyclingcadencesensor.h \
    src/datawriter.h \
    src/facilities.h \
    src/locationsensor.h \
    src/heartratemonitor.h

OTHER_FILES += \
    qml/cover/CoverDetailItem.qml \
    qml/cover/CoverPage.qml \
    qml/pages/AwesomeText.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    qml/pages/VintageIconSwitch.qml \
    qml/pages/VintageText.qml \
    qml/pages/VintageText7.qml \
    qml/pages/VintageText14.qml \
    qml/pages/VintageView.qml \
    rpm/harbour-cyclingcomputer.changes.in \
    rpm/harbour-cyclingcomputer.spec \
    rpm/harbour-cyclingcomputer.yaml \
    harbour-cyclingcomputer.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

INCLUDEPATH += $$_PRO_FILE_PWD_/include/QtBluetooth $$_PRO_FILE_PWD_/include

LIBS += -L$$_PRO_FILE_PWD_/lib -lQt5Bluetooth

btlib.files = $$_PRO_FILE_PWD_/lib/libQt5Bluetooth.so
btlib.path = /usr/share/harbour-cyclingcomputer/lib

INSTALLS += btlib

DISTFILES += \
    qml/Main.qml

RESOURCES += \
    resource.qrc
