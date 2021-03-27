import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 600
    height: 300
    anchors.fill: parent

    title: qsTr("Counter Strike IPv6 Settings")

    Binding {
        target: backEnd
        property: "ipv4Port"
        value: ipv4Port.value
    }
    Binding {
        target: backEnd
        property: "ipv6Port"
        value: ipv6Port.value
    }

    SpinBox {
        id: ipv4Port
        height: 50
        anchors.top: parent.top
        editable: true
        to: 9999999
        anchors.topMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20
        transformOrigin: Item.Center
        anchors.left: label.right
        anchors.leftMargin: 20
        value: backEnd.ipv4Port
        // @disable-check M222
        textFromValue: function (value) {
            return value
        }
    }

    Label {
        id: label
        height: 50
        text: qsTr("IPv4 Port")
        anchors.left: parent.left
        anchors.top: parent.top
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 20
        anchors.topMargin: 20
    }

    Label {
        id: label1
        height: 50
        text: qsTr("IPv6 Port")
        anchors.left: parent.left
        anchors.top: ipv4Port.bottom
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }

    SpinBox {
        id: ipv6Port
        height: 50
        to: 9999999
        anchors.left: ipv4Port.left
        anchors.right: parent.right
        anchors.top: ipv4Port.bottom
        editable: false
        transformOrigin: Item.Center
        anchors.rightMargin: 20
        anchors.topMargin: 20
        anchors.leftMargin: 0
        value: backEnd.ipv6Port
        // @disable-check M222
        textFromValue: function (value) {
            return value
        }
    }
}
