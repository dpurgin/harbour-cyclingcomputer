import QtQuick 2.0

import Sailfish.Silica 1.0

AwesomeText {
    id: root

    property bool busy: false
    property bool checked: false    

    color: {
        if (busy)
            return d.busyChecked? d.checkedColor: d.uncheckedColor;
        else
            return checked? d.checkedColor: d.uncheckedColor;
    }
    font.pixelSize: Theme.fontSizeLarge

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter

    MouseArea {
        anchors.fill: parent

        onClicked: root.clicked()
    }

    signal clicked()

    onCheckedChanged: {
        d.busyChecked = checked
    }

    Timer {
        interval: 500
        repeat: true
        running: busy
        triggeredOnStart: true

        onTriggered: d.busyChecked = !d.busyChecked
    }

    QtObject {
        id: d

        readonly property color checkedColor: Qt.rgba(0, 0, 0, 1)
        readonly property color uncheckedColor: Qt.rgba(0, 0, 0, 0.05)

        property bool busyChecked: false
    }
}
