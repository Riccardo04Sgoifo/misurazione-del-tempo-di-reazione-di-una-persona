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
    height: 480
    minimumWidth: 860
    minimumHeight: 480
    visible: true
    title: qsTr("Hello World")

    font.pointSize: 12

    Connections {
        target: mainLogic;
        function onErrorDialogRequested(error) {
            errorDialog.open(error);
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

    GridLayout {
        id: mainGrid
        rows: 2
        columns: 2
        anchors.fill: parent
        columnSpacing: 5
        anchors.margins: 5


        LightCanvas {
            id: canvas
            Layout.row: 0; Layout.column: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumWidth: 640
            Layout.maximumHeight: 600
            color: "#ffffff"
        }


       ColumnLayout {
            id: controlPanel
            Layout.maximumHeight: 600
            Layout.maximumWidth: 400
            Layout.alignment: Qt.AlignTop
            Layout.margins: 5
            spacing: 5

            RowLayout {
                id: startPanel
                Layout.fillWidth: true
                Layout.fillHeight: true

                Button {
                    text: "Inizia"
                    onClicked: {
                        mainLogic.startAttempts();
                    }
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
                Layout.fillHeight: true

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
            }

            RowLayout {
                id: lightNumPanel

                Layout.fillHeight: true
                Layout.fillWidth: true

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

                Layout.fillHeight: true
                Layout.fillWidth: true


                CheckBox {
                    text: "evita le ripetizioni"

                    checked: mainLogic.noReps

                    onCheckedChanged: {
                        mainLogic.setNoReps(checked);
                    }
                }
            }

        }

        ScoreTable {
            id: scoreLayout
        }

    }



}
