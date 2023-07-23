import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import Basic


Item {
    Layout.alignment: Qt.AlignTop
    Layout.fillWidth: true
    Layout.minimumHeight: 200
    Layout.fillHeight: true

    anchors.margins: 5

    ScrollView {
        id: scrollView

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: statisticsPanel.left



        STextEdit {
            anchors.fill: parent
            text: mainLogic.scoreBoardText

            onTextChanged: {
                mainLogic.setScoreBoardText(text);
                scrollView.ScrollBar.vertical.position = 1.0 - scrollView.ScrollBar.vertical.size;
            }

            readOnly: false
            wrapMode: TextEdit.Wrap

            selectByMouse: true


        }
    }

    Rectangle {
        id: statisticsPanel

        width: 200

        //anchors.left: scrollView.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        radius: 10

        color: "red"

        // Mean

        RowLayout {

            id: meanDisplay
            anchors.margins: 5

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {

                text: "Media: 234ms"


            }

            Button {

                implicitWidth: height

                text: ""
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter



                Image {

                    anchors.centerIn: parent
                    source: "qrc:///images/reload.png"
                    fillMode: Image.PreserveAspectFit
                    sourceSize.height: parent.background.height - 6
                    height: sourceSize.height
                    verticalAlignment: Image.AlignVCenter
                }


            }

        }

        // Error count

        RowLayout {

            id: errorDisplay
            anchors.margins: 5

            anchors.top: meanDisplay.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {

                text: "Errori: 3"


            }

            Button {

                implicitWidth: height

                text: ""
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter



                Image {

                    anchors.centerIn: parent
                    source: "qrc:///images/reload.png"
                    fillMode: Image.PreserveAspectFit
                    sourceSize.height: parent.background.height - 6
                    height: sourceSize.height
                    verticalAlignment: Image.AlignVCenter
                }


            }

        }

        // Best time

        RowLayout {

            id: bestTimeDisplay
            anchors.margins: 5

            anchors.top: errorDisplay.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {

                text: "Tempo migliore: 125ms"


            }

            Button {

                implicitWidth: height

                text: ""
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter



                Image {

                    anchors.centerIn: parent
                    source: "qrc:///images/reload.png"
                    fillMode: Image.PreserveAspectFit
                    sourceSize.height: parent.background.height - 6
                    height: sourceSize.height
                    verticalAlignment: Image.AlignVCenter
                }


            }

        }

        // Top percentage

        RowLayout {

            id: topPercentageDisplay
            anchors.margins: 5

            anchors.top: bestTimeDisplay.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {

                text: "percentuale miglire: 51%"


            }

            Button {

                implicitWidth: height

                text: ""
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter



                Image {

                    anchors.centerIn: parent
                    source: "qrc:///images/reload.png"
                    fillMode: Image.PreserveAspectFit
                    sourceSize.height: parent.background.height - 6
                    height: sourceSize.height
                    verticalAlignment: Image.AlignVCenter
                }


            }

        }

    }
}
