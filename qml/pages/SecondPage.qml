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

    Rectangle {
        anchors.fill: parent

        color: '#7d8c73'
    }

    Column
    {
        anchors.fill: parent

        Item {
            width: parent.width
            height: parent.height * 0.25

            Text {
                anchors.left: parent.left
                anchors.horizontalCenter: parent.horizontalCenter

                text: LocationSensor.altitude

                font.pixelSize: Theme.fontSizeLarge * 2

                color: Theme.primaryColor
            }
        }

        Item {
            width: parent.width
            height: parent.height * 0.5

            Text {
                anchors.centerIn: parent

                horizontalAlignment: Text.AlignHCenter

                text: Math.round(LocationSensor.speed * 180 / 5) / 10

                font.pixelSize: Theme.fontSizeHuge * 2

                color: Theme.primaryColor
            }
        }

        Item {
            width: parent.width
            height: parent.height * 0.25

            Text {
                id: hrmValue

                anchors.left: parent.left
                anchors.right: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                text: HeartRateMonitor.value

                horizontalAlignment: Text.AlignHCenter

                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeLarge * 2
            }

            Text {
                anchors.left: parent.left
                anchors.right: parent.horizontalCenter

                anchors.top: hrmValue.bottom

                text: 'HRM'

                horizontalAlignment: Text.AlignHCenter
                color: Theme.secondaryColor
            }

            Text {
                id: cadValue

                anchors.left: parent.horizontalCenter
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                horizontalAlignment: Text.AlignHCenter

                text: CyclingCadenceSensor.value

                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeLarge * 2
            }

            Text {
                anchors.left: parent.horizontalCenter
                anchors.right: parent.right

                anchors.top: cadValue.bottom

                text: 'CAD'

                horizontalAlignment: Text.AlignHCenter
                color: Theme.secondaryColor
            }
        }
    }
}





