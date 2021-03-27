import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window
    visible: true
    width: 600
    height: 300
    title: qsTr("CS IPv6 Manager")
    flags: Qt.Window | Qt.CustomizeWindowHint

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        MouseArea {
            id: mouseRegion
            anchors.fill: parent;
            property variant clickPos: "1,1"

            onPressed: {
                clickPos  = Qt.point(mouse.x,mouse.y)
            }

            onPositionChanged: {
                var delta = Qt.point(mouse.x-clickPos.x, mouse.y-clickPos.y)
                window.x += delta.x;
                window.y += delta.y;
            }
        }

        ToolButton {
            id: toolButton
            text: "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                drawer.open()
            }
        }

        Label {
            text: view.currentItem.title
            anchors.centerIn: parent
        }

        ToolButton {
            id: closeButton
            text: "X"
            anchors.right: parent.right
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                close()
            }
        }
    }    

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Server")
                width: parent.width
                onClicked: {
                    view.currentIndex = 0
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Client")
                width: parent.width
                onClicked: {
                    view.currentIndex = 1
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    view.currentIndex = 2
                    drawer.close()
                }
            }
        }
    }

    SwipeView {
        id: view
        currentIndex: 1
        anchors.fill: parent
        interactive: false

        Page {
            title: serverPage.title
            PageServer
            {
                id: serverPage
            }
        }
        Page {
            title: clientPage.title
            PageClient
            {
                id: clientPage
            }
        }
        Page {
            title: settingsPage.title
            PageSettings
            {
                id: settingsPage
            }
        }
    }
}
