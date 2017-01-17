import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.cyclingcomputer.Sensors 1.0
import harbour.cyclingcomputer.Facilities 1.0

Page {
    id: page    

    Rectangle {
        anchors.fill: parent

        color: '#7d8c73'
    }

    Item {
        id: toolbar

        width: parent.width
        height: parent.height * 0.1

        VintageText7 {
            id: timeLabel

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: Theme.horizontalPageMargin

            font.pixelSize: Theme.fontSizeLarge

            backgroundText: "00:00:00"
        }

        VintageIconSwitch {
            id: gpsSwitch

            busy: !LocationSensor.reliable
            checked: LocationSensor.available

            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: bluetoothSwitch.left

            width: height

            text: "\uf14e"
        }

        VintageIconSwitch {
            id: bluetoothSwitch

            busy: Bluetooth.busy
            checked: Bluetooth.powered

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            anchors.right: saveSwitch.left

            width: height

            text: "\uf293"
        }

        VintageIconSwitch {
            id: saveSwitch

            anchors.top: parent.top
            anchors.bottom: parent.bottom

            anchors.right: parent.right

            width: height

            text: "\uf0c7"

            checked: dataWriter.active
            busy: dataWriter.active && !LocationSensor.reliable

            onClicked: dataWriter.active = !dataWriter.active;
        }

        Timer {
            id: timer

            running: true
            repeat: true
            interval: 1000
            triggeredOnStart: true

            onTriggered: {
                var dt = new Date();
                timeLabel.foregroundText = dt.toTimeString().substring(0, 8);
            }
        }
    }

    Item {
        id: speedItem

        width: parent.width
        height: parent.height * 0.3

        anchors.top: toolbar.bottom

        VintageText7 {
            id: spdValue

            anchors.centerIn: parent

            backgroundText: "8.8.8"

            font.pixelSize: Theme.fontSizeExtraLarge * 3.5
        }

        VintageText14 {
            id: spdLabel

            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            font.pixelSize: Theme.fontSizeLarge

            backgroundText: "~~~~~~"
        }

        state: "spd"
        states: [
            State {
                name: "spd"

                PropertyChanges {
                    target: spdLabel
                    foregroundText: "KMH"
                }

                PropertyChanges {
                    target: spdValue
                    foregroundText:
                        LocationSensor.available? speedItem.formatSpeed(LocationSensor.speed): "---"
                }
            },

            State {
                name: "avgspd"

                PropertyChanges {
                    target: spdLabel
                    foregroundText: "AVGKMH"
                }

                PropertyChanges {
                    target: spdValue
                    foregroundText: speedItem.formatSpeed(LocationSensor.averageSpeed)
                }
            },

            State {
                name: "maxspd"

                PropertyChanges {
                    target: spdLabel
                    foregroundText: "MAXKMH"
                }

                PropertyChanges {
                    target: spdValue
                    foregroundText: speedItem.formatSpeed(LocationSensor.maxSpeed)
                }
            },

            State {
                name: "dist"

                PropertyChanges {
                    target: spdLabel
                    foregroundText: "KM"
                }

                PropertyChanges {
                    target: spdValue
                    foregroundText: speedItem.formatDistance(LocationSensor.distance)
                }
            }
        ]

        MouseArea {
            anchors.fill: parent

            onClicked: speedItem.state = page.getNextState(speedItem.state, speedItem.states)
        }

        function formatDistance(m)
        {
            var kmDistance = m / 1000;

            if (kmDistance < 10)
                kmDistance = Math.round(kmDistance * 100) / 100;
            else if (kmDistance < 100)
                kmDistance = Math.round(kmDistance * 10) / 10;
            else
                kmDistance = Math.round(kmDistance);

            if (kmDistance != 0)
            {
                kmDistance = String(kmDistance);

                while (kmDistance.length < 3)
                    kmDistance += "0";
            }

            return kmDistance;
        }

        function formatSpeed(mps)
        {
            var val = Math.round(mps * 18 / 5 * 10) / 10;

            if (Math.round(val) == val)
                val = String(val) + ".0";

            return val;
        }
    }

    Item {
        id: elevationItem

        anchors.bottom: hrmItem.top
        anchors.bottomMargin: 3 * Theme.paddingLarge

        anchors.left: parent.left
        anchors.leftMargin: Theme.horizontalPageMargin

        width: Math.max(elevationValue.width, elevationLabel.width)
        height: elevationValue.height + elevationLabel.height

        VintageText7 {
            id: elevationValue

            anchors.bottom: elevationLabel.top
            anchors.bottomMargin: Theme.paddingLarge
            anchors.left: parent.left
            font.pixelSize: Theme.fontSizeExtraLarge * 2

            foregroundText: LocationSensor.available? LocationSensor.altitude: '----'
            backgroundText: "8888"

            horizontalAlignment: Text.AlignLeft
        }

        VintageText14 {
            id: elevationLabel

            anchors.bottom: parent.bottom
            anchors.left: parent.left

            font.pixelSize: Theme.fontSizeLarge

            backgroundText: "~~~~"
            foregroundText: "ELEV"
        }

        state: "elev"
        states: [
            State {
                name: "elev"

                PropertyChanges {
                    target: elevationLabel
                    foregroundText: "ELEV"
                }

                PropertyChanges {
                    target: elevationValue
                    foregroundText: LocationSensor.available? LocationSensor.altitude: "----"
                }
            },

            State {
                name: "gain"

                PropertyChanges {
                    target: elevationLabel
                    foregroundText: "GAIN"
                }

                PropertyChanges {
                    target: elevationValue
                    foregroundText: LocationSensor.elevationGain
                }
            }
        ]

        MouseArea {
            anchors.fill: parent

            onClicked: elevationItem.state = page.getNextState(elevationItem.state,
                                                               elevationItem.states)
        }
    }

    Item {
        id: gradientItem

        anchors.bottom: cadItem.top
        anchors.bottomMargin: 3 * Theme.paddingLarge

        anchors.right: parent.right
        anchors.rightMargin: Theme.horizontalPageMargin

        VintageText7 {
            id: gradientValue

            anchors.bottom: gradientLabel.top
            anchors.bottomMargin: Theme.paddingLarge
            anchors.right: parent.right
            font.pixelSize: Theme.fontSizeExtraLarge * 2

            foregroundText: {
                var result = '';

                if (LocationSensor.available)
                {
                    var gradient = Math.abs(LocationSensor.gradient * 100);

                    result = (gradient < 10? Math.round(gradient * 10) / 10: Math.round(gradient));
                }
                else
                    result = '--';

                return result;
            }

            backgroundText: "8.8"
        }

        VintageText14 {
            id: gradientLabel

            anchors.bottom: parent.bottom
            anchors.right: parent.right

            font.pixelSize: Theme.fontSizeLarge

            backgroundText: "~~~~~"
            foregroundText: {
                if (LocationSensor.gradient > 0)
                    return "GRAD<";
                else if (LocationSensor.gradient < 0)
                    return  "GRAD>";

                return "GRAD-"
            }
        }
    }

    Item {
        id: hrmItem

        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.paddingLarge

        anchors.left: parent.left
        anchors.leftMargin: Theme.horizontalPageMargin

        width: Math.max(hrmValue.width, hrmLabel.width)
        height: hrmValue.height + hrmLabel.height

        VintageText7 {
            id: hrmValue

            anchors.bottom: hrmLabel.top
            anchors.bottomMargin: Theme.paddingLarge
            anchors.left: parent.left
            font.pixelSize: Theme.fontSizeExtraLarge * 2

            backgroundText: "888"

            horizontalAlignment: Text.AlignLeft
        }

        VintageText14 {
            id: hrmLabel

            anchors.bottom: parent.bottom
            anchors.left: parent.left

            font.pixelSize: Theme.fontSizeLarge

            backgroundText: "~~~.~~~"
            foregroundText: "HRM"

            horizontalAlignment: Text.AlignLeft
        }

        state: "cur"
        states: [
            State {
                name: "cur"

                PropertyChanges {
                    target: hrmLabel
                    foregroundText: "HRM"
                }

                PropertyChanges {
                    target: hrmValue
                    foregroundText: HeartRateMonitor.connected && HeartRateMonitor.attached?
                                        HeartRateMonitor.value:
                                        "---"
                }
            },

            State {
                name: "avg"

                PropertyChanges {
                    target: hrmLabel
                    foregroundText: "AVGHRM"
                }

                PropertyChanges {
                    target: hrmValue
                    foregroundText: HeartRateMonitor.averageValue
                }
            },

            State {
                name: "max"

                PropertyChanges {
                    target: hrmLabel
                    foregroundText: "MAXHRM"
                }

                PropertyChanges {
                    target: hrmValue
                    foregroundText: HeartRateMonitor.maxValue
                }
            }
        ]

        MouseArea {
            anchors.fill: parent

            onClicked: hrmItem.state = page.getNextState(hrmItem.state, hrmItem.states)
        }
    }

    Item {
        id: cadItem

        anchors.bottom: parent.bottom
        anchors.bottomMargin: Theme.paddingLarge

        anchors.right: parent.right
        anchors.rightMargin: Theme.horizontalPageMargin

        width: Math.max(cadValue.width, cadLabel.width)
        height: cadValue.height + cadLabel.height

        VintageText7 {
            id: cadValue

            anchors.bottom: cadLabel.top
            anchors.bottomMargin: Theme.paddingLarge
            anchors.right: parent.right
            font.pixelSize: Theme.fontSizeExtraLarge * 2

            foregroundText: CyclingCadenceSensor.connected? CyclingCadenceSensor.value: "---"
            backgroundText: "888"
        }

        VintageText14 {
            id: cadLabel

            anchors.bottom: parent.bottom
            anchors.right: parent.right

            font.pixelSize: Theme.fontSizeLarge

            backgroundText: "~~~~~~"
            foregroundText: "CAD"
        }

        state: "cur"
        states: [
            State {
                name: "cur"

                PropertyChanges {
                    target: cadLabel
                    foregroundText: "CAD"
                }

                PropertyChanges {
                    target: cadValue
                    foregroundText: CyclingCadenceSensor.connected?
                                        CyclingCadenceSensor.value:
                                        "---"
                }
            },

            State {
                name: "avg"

                PropertyChanges {
                    target: cadLabel
                    foregroundText: "AVGCAD"
                }

                PropertyChanges {
                    target: cadValue
                    foregroundText: CyclingCadenceSensor.averageValue
                }
            },

            State {
                name: "max"

                PropertyChanges {
                    target: cadLabel
                    foregroundText: "MAXCAD"
                }

                PropertyChanges {
                    target: cadValue
                    foregroundText: CyclingCadenceSensor.maxValue
                }
            }
        ]

        MouseArea {
            anchors.fill: parent

            onClicked: cadItem.state = page.getNextState(cadItem.state, cadItem.states)
        }
    }

    function getNextState(currentState, states)
    {
        var state = "";

        for (var i = 0; i < states.length; ++i)
        {
            if (states[i].name === currentState)
            {
                state = (i === states.length - 1? states[0].name: states[i + 1].name);
                break;
            }
        }

        return state;
    }
}
