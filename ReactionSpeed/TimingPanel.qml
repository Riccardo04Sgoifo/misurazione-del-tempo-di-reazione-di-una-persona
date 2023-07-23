import QtQuick 2.15
import QtQuick.Controls.Fusion
import Basic
import QtQuick.Layouts

ColumnLayout {

    RowLayout {
        id: timingModeBarNText
        Layout.preferredHeight: timingModeBar.implicitHeight
        Layout.fillWidth: true

        SText {
            text: "intervallo:"
        }

        TabBar {
            id: timingModeBar
            Layout.fillWidth: true

            TabButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "fisso"
            }

            TabButton {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "casuale"
            }

            onCurrentIndexChanged: {
                if (currentIndex === 0) {
                    fixedSpinBox.setUsedValue();
                }

                if (currentIndex === 1) {
                    randomSpinBox.setUsedValue();
                }
            }
        }
    }

    SwipeView {
        id: timingModePanels
        currentIndex: timingModeBar.currentIndex

        Layout.fillWidth: true
        Layout.maximumHeight: 100
        height: randomModeText.height + randomFromSpinBox.height + 10
        Layout.alignment: Qt.AlignTop


        clip: true



        Item {
            id: fixedModeTab
            //Layout.fillWidth: true
            //Layout.fillHeight: true
            width: timingModePanels.width
            height: timingModePanels.height
            anchors.margins: 5

            Rectangle {
                anchors.fill: parent
                color: "#272727"
                radius: 10
            }
            ColumnLayout {
                anchors.fill: parent

                anchors.margins: 5

                RowLayout {

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop

                    SText {
                        text: "Intervallo di:"
                    }

                    SSpinBox {

                        id: fixedSpinBox

                        from: 0
                        to: 10000
                        value: 1000
                        stepSize: 100

                        function setUsedValue() {
                            mainLogic.setRandomIntervalFrom(value);
                            mainLogic.setRandomIntervalTo(value);
                        }

                        onValueChanged: {
                            if (value === 0){
                                noTimeCheckBox.checked = true;
                            }
                            else {
                                noTimeCheckBox.prevValue = value;
                                noTimeCheckBox.checked = false;
                            }
                            setUsedValue();
                        }
                    }
                }

                CheckBox {

                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    id: noTimeCheckBox

                    text: "Senza intervallo"

                    property int prevValue: 0

                    onCheckedChanged: {

                        if (checked) {
                            prevValue = fixedSpinBox.value;

                            fixedSpinBox.value = 0;
                        }
                        else {
                            fixedSpinBox.value = prevValue;

                            prevValue = 0;
                        }

                    }
                }

            }
        }

        Item {
            id: randomModeTab

            //Layout.fillWidth: true
            //Layout.fillHeight: true
            width: timingModePanels.width
            height: timingModePanels.height

            anchors.margins: 5

            Rectangle {
                anchors.fill: parent
                color: "#272727"
                radius: 10
            }

            ColumnLayout {
                anchors.fill: parent

                anchors.margins: 5

                SText {
                    id: randomModeText
                    text: "Intervallo casuale da"
                    Layout.alignment: Qt.AlignTop
                }

                RowLayout {

                    Layout.alignment: Qt.AlignTop

                    id: randomSpinBox

                    function setUsedValue() {
                        mainLogic.setRandomIntervalFrom(randomFromSpinBox.value);
                        mainLogic.setRandomIntervalTo(randomToSpinBox.value);
                    }


                    SSpinBox {

                        id: randomFromSpinBox

                        from: 0
                        to: 10000
                        stepSize: 100

                        value: mainLogic.randomIntervalFrom

                        onValueChanged: {
                            if (value > randomToSpinBox.value) {
                                randomToSpinBox.value = value;
                            }
                            randomSpinBox.setUsedValue();
                        }
                    }

                    SText {
                        text: "a"
                    }

                    SSpinBox {

                        id: randomToSpinBox

                        from: 0
                        to: 10000
                        stepSize: 100

                        value: mainLogic.randomIntervalTo

                        onValueChanged: {
                            if (randomFromSpinBox.value > value) {
                                randomFromSpinBox.value = value;
                            }
                            randomSpinBox.setUsedValue();
                        }
                    }
                }
            }
        }
    }
}
