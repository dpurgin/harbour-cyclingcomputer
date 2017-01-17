import QtQuick 2.0

import Sailfish.Silica 1.0

Item {
    width: parent.width
    height: Math.max(labelLabel.height, valueLabel.height) + 2 * Theme.paddingSmall

    property alias label: labelLabel.text
    property alias value: valueLabel.text

    Label {
        id: labelLabel

        anchors.verticalCenter: parent.verticalCenter

        anchors.left: parent.left
        anchors.leftMargin: Theme.paddingSmall

        anchors.right: parent.horizontalCenter
        anchors.rightMargin: Theme.paddingSmall

        color: Theme.secondaryColor
        font.pixelSize: Theme.fontSizeSmall

        horizontalAlignment: Text.AlignRight

        truncationMode: TruncationMode.Fade
    }

    Label {
        id: valueLabel

        anchors.verticalCenter: parent.verticalCenter

        anchors.left: parent.horizontalCenter

        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingSmall

        horizontalAlignment: Text.AlignLeft

        truncationMode: TruncationMode.Fade

        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeSmall
    }
}
