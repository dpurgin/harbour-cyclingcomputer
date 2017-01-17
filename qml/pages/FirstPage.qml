/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.cyclingcomputer.Sensors 1.0

Page {
    id: page

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
            MenuItem {
                text: qsTr("Vintage")
                onClicked: pageStack.push(Qt.resolvedUrl("VintageView.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader {
                text: qsTr('Cycling Cadence')
            }

            TextField {
                id: cadenceSensorAddress

                validator: RegExpValidator {
                    regExp: /([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}/
                }

                width: parent.width

                placeholderText: "Cadence sensor address"
                label: "Cadence sensor address"

                onTextChanged: {
                    CyclingCadenceSensor.address = text
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: Theme.paddingMedium

                Button {
                    text: "Connect"

                    onClicked: {
                        CyclingCadenceSensor.connectSensor();
                    }
                }

                Button {
                    text: "Disconnect"

                    onClicked: {
                        CyclingCadenceSensor.reconnect = false;
                        CyclingCadenceSensor.disconnectSensor();
                    }
                }
            }

            TextSwitch {
                text: "Reconnect automatically"
                checked: CyclingCadenceSensor.reconnect

                onCheckedChanged: CyclingCadenceSensor.reconnect = checked
            }

            Column {
                width: parent.width

                visible: CyclingCadenceSensor.connected

                DetailItem {
                    label: "Name"
                    value: CyclingCadenceSensor.name
                }

                DetailItem {
                    label: "Measurement"
                    value: CyclingCadenceSensor.value
                }

                DetailItem {
                    visible: CyclingCadenceSensor.batterySupported

                    label: "Battery"
                    value: CyclingCadenceSensor.batteryLevel + "%"
                }
            }

            SectionHeader {
                text: "Heart rate"
            }

            TextField {
                id: heartRateMonitorAddress

                validator: RegExpValidator {
                    regExp: /([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}/
                }

                width: parent.width

                placeholderText: "Heart rate monitor address"
                label: "Heart rate monitor address"

                onTextChanged: {
                    HeartRateMonitor.address = text
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter

                spacing: Theme.paddingMedium

                Button {
                    text: "Connect"

                    onClicked: {
                        HeartRateMonitor.connectSensor();
                    }
                }

                Button {
                    text: "Disconnect"

                    onClicked: {
                        HeartRateMonitor.reconnect = false;
                        HeartRateMonitor.disconnectSensor();
                    }
                }
            }

            TextSwitch {
                text: "Reconnect automatically"

                checked: HeartRateMonitor.reconnect

                onCheckedChanged: HeartRateMonitor.reconnect = checked
            }

            Column {
                width: parent.width

                visible: HeartRateMonitor.connected

                DetailItem {
                    label: "Name"
                    value: HeartRateMonitor.name
                }

                DetailItem {
                    label: "Attached"
                    value: HeartRateMonitor.attached
                }

                DetailItem {
                    label: "Measurement"
                    value: HeartRateMonitor.value
                }

                DetailItem {
                    visible: HeartRateMonitor.batterySupported

                    label: "Battery"
                    value: HeartRateMonitor.batteryLevel + "%"
                }
            }

        }
    }

    Component.onCompleted: {
        cadenceSensorAddress.text = "F6:CA:A0:CC:CB:16";
        heartRateMonitorAddress.text = "D1:97:0A:08:EC:DF";
    }
}

