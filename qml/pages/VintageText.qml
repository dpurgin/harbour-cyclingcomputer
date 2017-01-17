import QtQuick 2.2

Text {
    id: background

    property alias horizontalAlignment: foreground.horizontalAlignment

    property alias backgroundText: background.text
    property alias foregroundText: foreground.text

    color: Qt.rgba(0, 0, 0, 0.05)

    Text {
        id: foreground

        anchors.left: background.left
        anchors.right: background.right

        horizontalAlignment: Text.AlignRight

        font.family: background.font.family
        font.pixelSize: background.font.pixelSize

        color: "black"
    }
}
