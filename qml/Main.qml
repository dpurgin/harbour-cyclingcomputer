import QtQuick 2.2
import Sailfish.Silica 1.0
import "pages"

import harbour.cyclingcomputer.Sensors 1.0

import Sailfish.Media 1.0

ApplicationWindow
{
    initialPage: Component { FirstPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All

    ScreenBlank {
        interval: 5000
        suspend: true
    }

    Timer {
        id: dataWriterTimer

        interval: 1000
        repeat: true
        running: true

        onTriggered: {
            if (dataWriter.active && LocationSensor.reliable)
            {
                dataWriter.write(LocationSensor.longitude,
                                 LocationSensor.latitude,
                                 LocationSensor.altitude,
                                 LocationSensor.speed,
                                 LocationSensor.averageSpeed,
                                 LocationSensor.distance,
                                 LocationSensor.elevationGain,
                                 LocationSensor.gradient,
                                 HeartRateMonitor.value,
                                 HeartRateMonitor.averageValue,
                                 CyclingCadenceSensor.value,
                                 CyclingCadenceSensor.averageValue);
            }
        }
    }

    FontLoader {
        source: Qt.resolvedUrl("qrc:/fonts/dseg14.ttf")
    }

    FontLoader {
        source: Qt.resolvedUrl("qrc:/fonts/dseg7.ttf")
    }

    FontLoader {
        source: Qt.resolvedUrl("qrc:/fonts/fontawesome.ttf")
    }
}


