import QtQuick 2.4

Item {

    width: 1024
    height: 728

    id: root

    property string messText: "Test"
    property alias mouseArea: mouseArea
    property alias rectangle: rectangle
    property alias text: text

    anchors.fill: parent

    Rectangle {

        id: rectangle

        x: 412
        y: 40

        width: 200
        height: 200

        color: "#ffb508"

        objectName: "messageText"

    }

    Text {

        id: text

        x: 453
        y: 499

        text: qsTr( messText )

        font.pixelSize: 18

        objectName: "messageText"

    }

    MouseArea {

        id: mouseArea

        anchors.fill: parent

    }

}


