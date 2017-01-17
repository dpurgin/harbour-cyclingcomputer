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

CoverBackground {
    id: page

    Column {
        id: content

        anchors.fill: parent

        CoverDetailItem {
            label: "Avg. speed"
            value: String(Math.round(LocationSensor.averageSpeed * 180 / 5) / 10) + " km/h";
        }

        CoverDetailItem {
            label: "Distance"
            value: String(Math.round(LocationSensor.distance / 10) / 100) + " km"
        }

        CoverDetailItem {
            label: "Elev. gain"
            value: LocationSensor.elevationGain + " m"
        }

        CoverDetailItem {
            label: "Avg. HRM"
            value: HeartRateMonitor.averageValue > 0? HeartRateMonitor.averageValue + " bpm": "n/a"
        }

        CoverDetailItem {
            label: "Avg. cad."
            value: CyclingCadenceSensor.averageValue > 0?
                       CyclingCadenceSensor.averageValue + " rpm":
                       "n/a"
        }

        Component.onCompleted: {
            for (var i = 0; i < children.length; ++i)
                children[i].height = height / children.length;
        }
    }
}


