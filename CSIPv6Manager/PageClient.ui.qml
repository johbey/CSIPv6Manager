import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 600
    height: 300
    anchors.fill: parent

    title: qsTr("Counter Strike IPv6 Client")

    Connections {
        target: backEnd
        // @disable-check M222
        function onClientConnected() {
            connectButton.text = qsTr("Stop")
            progressBar.visible = true
        }
        // @disable-check M222
        function onClientDisconnected() {
            connectButton.text = qsTr("Start")
            progressBar.visible = false
        }
    }

    TextField {
        id: textField
        height: 50
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        anchors.topMargin: 20
        anchors.right: connectButton.left
        anchors.rightMargin: 20
        transformOrigin: Item.Center
        anchors.left: pasteButton.right
        anchors.leftMargin: 20
        placeholderText: qsTr("Server IPv6 Address")
        selectByMouse: true
    }

    Button {
        id: connectButton
        x: 433
        width: 115
        height: 50
        text: qsTr("Start")
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        // @disable-check M222
        onClicked: backEnd.toggleClient(textField.text)
    }

    ProgressBar {
        id: progressBar
        height: 20
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: connectButton.bottom
        indeterminate: true
        enabled: true
        hoverEnabled: false
        scale: 1
        value: 0
        anchors.rightMargin: 20
        wheelEnabled: false
        focusPolicy: Qt.NoFocus
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }

    Button {
        id: pasteButton
        width: 115
        height: 50
        text: qsTr("Paste")
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 20
        // @disable-check M223
        onClicked: {
            // @disable-check M222
            textField.clear()
            // @disable-check M222
            textField.paste()
        }
    }

    Label {
        id: errorLabel
        y: 127
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.bottomMargin: 20
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        text: backEnd.clientError
    }
}
