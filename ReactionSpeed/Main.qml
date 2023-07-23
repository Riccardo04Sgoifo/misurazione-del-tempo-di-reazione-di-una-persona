import QtQuick
import QtQml
import QtQuick.Window
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Effects
import Basic

ApplicationWindow {
    id: mainWindow

    width: 640
    height: 540
    minimumWidth: 960
    minimumHeight: 480
    visible: true
    title: qsTr("Hello World")

    font.pointSize: 12

    onClosing: {
        mainLogic.stopAttempts();
    }

    Connections {
        target: mainLogic;
        function onErrorDialogRequested(error) {
            errorDialog.openDialog(error);
        }
    }

    menuBar: MenuBar {

//        background: Rectangle{
//            implicitHeight: 10
//            color: Material.backgroundColor
//        }

        Menu {
            title: "File"
            Action {
                text: "Esci"
                onTriggered: {
                    mainWindow.close();
                }
            }
        }

        Menu {
            title: "Strumenti"
            Action {
                text: "Connetti"
                onTriggered: {
                    connectDialog.open();
                }
            }
        }
    }

    Dialog {
        id: errorDialog
        title: "something went wrong"

        anchors.centerIn: Overlay.overlay

        standardButtons: Dialog.Ok | Dialog.Cancel

        function openDialog(error) {
            textux.text = error;
            open();
        }

        SText {
            id: textux
            anchors.fill: parent
            text: "some error occurred";
        }
    }

    Dialog {
        id: connectDialog
        title: "connetti esp8266"

        margins: 100

        onOpened: {
            mainLogic.requestUpdateAvailableSerialPorts();

        }

        ColumnLayout {

            anchors.fill: parent

            ComboBox {

                Layout.fillHeight: true
                Layout.fillWidth: true

                model: mainLogic.availableSerialPorts;


                onCurrentValueChanged: {
                    mainLogic.setCurrentSerialPort(currentValue);
                }
            }

            STextEdit {

                Layout.fillHeight: true
                Layout.preferredWidth: 200

                text: mainLogic.currentSerialPortDescription

                activeFocusOnPress: false
                readOnly: true
                wrapMode: TextEdit.Wrap
            }
        }



        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            mainLogic.connectPort();
        }
    }

    Item {
        id: mainGrid
        anchors.fill: parent
        anchors.margins: 5




        LightCanvas {
            id: canvas
            anchors.top: parent.top
            anchors.right: controlPanel.left
            anchors.bottom: scoreTable.top
            anchors.left: parent.left

            Layout.minimumWidth: 640
            Layout.maximumHeight: 600
            Layout.minimumHeight: controlPanel.height
            color: "#ffffff"
        }


       Item {
            id: controlPanel
            height: 320
            width: 300


            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: scoreTable.top



            anchors.margins: 5
            anchors.rightMargin: 0

            RowLayout {
                id: startPanel

                anchors.top: parent.top
                anchors.left: parent.left


                Button {
                    text: "Inizia"

                    enabled: mainLogic.isConnected

                    onClicked: {
                        mainLogic.startAttempts();
                    }

                    ToolTip.visible: hovered && !enabled
                    ToolTip.delay: 100
                    ToolTip.text: "connetti la scheda prima di iniziare"
                }

                SText {
                    text: "inizia tra:"

                }

                SSpinBox {
                    from: 0
                    to: 10000
                    value: mainLogic.startDelay
                    editable: true
                    stepSize: 100

                    onValueChanged: {
                        mainLogic.setStartDelay(value);
                    }

                    timeMode: "SECONDS"
                }

            }

            RowLayout {
                id: attemptsPanel
                Layout.fillWidth: true

                //Layout.alignment: Qt.AlignTop || Qt.AlignBottom
                anchors.top: startPanel.bottom


                SText {
                    text: "numero di tentativi:"
                }

                SpinBox { // do not need ms and s
                    from: 1
                    to: 1000
                    value: mainLogic.attemptNum
                    editable: true
                    stepSize: 1

                    onValueChanged: {
                        mainLogic.setAttemptNum(value);
                    }

                }
            }

            TimingPanel {
                id: timingPanel
                anchors.top: attemptsPanel.bottom
                anchors.left: parent.left
                anchors.right: parent.right
            }

            RowLayout {
                id: lightNumPanel

                anchors.top: timingPanel.bottom
                anchors.topMargin: 10


                SText {
                    text: "numero di sensori: "
                }

                SpinBox {
                    from: 1
                    to: 4 // numero massimo di sensori
                    stepSize: 1
                    value: mainLogic.lightNum;

                    onValueChanged: {
                        mainLogic.setLightNum(value)
                    }
                }
            }

            RowLayout {

                id: noRepetitions

                anchors.top: lightNumPanel.bottom



                CheckBox {
                    text: "evita le ripetizioni"

                    checked: mainLogic.noReps

                    onCheckedChanged: {
                        mainLogic.setNoReps(checked);
                    }
                }
            }

            RowLayout {

                id: stopButtonLayout

                anchors.top: noRepetitions.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                DelayButton {
                    delay: 400
                    text: "Interrompi"

                    Layout.fillWidth: true

                    onActivated: {
                        mainLogic.stopAttempts();
                    }
                }
            }

        }

        ScoreTable {

            anchors.topMargin: 5

            anchors.right: controlPanel.left
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: controlPanel.bottom


            id: scoreTable
        }

    }



}
