import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: pageServer
    width: 600
    height: 450
    anchors.fill: parent
    title: qsTr("Counter Strike IPv6 Server")

    Connections {
        target: backEnd
        // @disable-check M222
        function onServerStarted() {
            buttonStartServer.text = qsTr("Stop Server")
            progressBar.visible = true
        }
        // @disable-check M222
        function onServerStopped() {
            buttonStartServer.text = qsTr("Start Server")
            progressBar.visible = false
        }
    }

    TextField {
        id: textField
        height: 50
        anchors.left: label.right
        anchors.right: buttonCopy.left
        anchors.top: buttonStartServer.bottom
        horizontalAlignment: Text.AlignHCenter
        hoverEnabled: false
        transformOrigin: Item.Center
        anchors.rightMargin: 20
        placeholderText: qsTr("Server IPv6 Address")
        anchors.topMargin: 20
        anchors.leftMargin: 20
        //readOnly: true
        text: backEnd.ipv6Address
        // @disable-check M222
        onTextChanged: textField.selectAll()
    }

    Button {
        id: buttonCopy
        x: 433
        width: 115
        height: 50
        text: qsTr("Copy")
        anchors.right: parent.right
        anchors.top: buttonStartServer.bottom
        anchors.rightMargin: 20
        anchors.topMargin: 20

        // @disable-check M223
        onClicked: {
            // @disable-check M222
            textField.selectAll()
            // @disable-check M222
            textField.copy()
        }
    }

    Label {
        id: label
        height: 50
        text: qsTr("Current IP")
        anchors.left: parent.left
        anchors.top: buttonStartServer.bottom
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }

    Button {
        id: buttonStartServer
        width: 115
        height: 50
        text: "Start Server"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 20
        anchors.topMargin: 20
        // @disable-check M222
        onClicked: backEnd.toggleServer()
    }

    ProgressBar {
        id: progressBar
        height: 20
        visible: false
        anchors.left: parent.left
        anchors.right: buttonStartServer.left
        anchors.top: buttonStartServer.top
        enabled: true
        wheelEnabled: false
        hoverEnabled: false
        focusPolicy: Qt.NoFocus
        scale: 1
        anchors.rightMargin: 20
        indeterminate: true
        anchors.leftMargin: 40
        anchors.topMargin: 15
        value: 0
    }

    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: label.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 20
        anchors.bottomMargin: 40
        anchors.leftMargin: 40
        anchors.rightMargin: 40
        model: backEnd.clients
        delegate: Item {
            x: 5
            width: parent.width - 1
            height: 20
            Row {
                id: row1
                Text {
                    text: modelData
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 11
                    color: "#ffffff"
                }
                spacing: 10
            }
        }
    }

    Label {
        id: errorLabel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.bottomMargin: 20
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        text: backEnd.serverError
    }
}
