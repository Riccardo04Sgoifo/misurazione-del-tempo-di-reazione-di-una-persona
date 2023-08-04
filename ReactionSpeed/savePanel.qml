import QtQuick 2.15
import QtQml
import QtQuick.Controls.Fusion

Item {

    Connections {
        target: mainLogic;
        function onAttemptNamesChanged(names){
            scoreListView.model.clear()

            console.log(names[0])

            for (let i = 0; i < Object.keys(names).length; i++){
                scoreListView.model.append({"name" :   names[i]/*names[i]*/, "mean" : "100ms"})
            }


        }
    }

    Rectangle {

        id: listSection

        color: "dark gray"
        radius: 5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: saveButton.top

        Component {
            id: scoreDelegate
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                height: 40
                Column {
                    Text { text: '<b>Nome:</b> ' + name }
                    Text { text: '<b>Media:</b> ' + mean }
                }
            }
        }

        ListView {

            id: scoreListView

            anchors.fill: parent

            model: ListModel {
                ListElement {
                    name: "Urpo Settimo"
                    mean: "243ms"
                }
                ListElement {
                    name: "Luigi Quattordicesimo"
                    mean: "1430ms"
                }
                ListElement {
                    name: "Papa Francesco"
                    mean: "2ms"
                }
            }

            delegate: scoreDelegate
            highlight: Rectangle { color: "lightSteelBlue"; radius: 5}
            focus: false
            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.StopAtBounds
            highlightFollowsCurrentItem: true

            onCurrentIndexChanged: {
                mainLogic.loadAttempt(currentIndex);
                console.log(currentIndex)
            }
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                var index = scoreListView.indexAt(mouseX, mouseY);
                if (index > -1) {
                    scoreListView.positionViewAtIndex(index, ListView.visible);
                    scoreListView.currentIndex = index;
                }


                scoreListView.forceActiveFocus();
            }
        }

    }

    Button {

        id: saveButton

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.bottom: parent.bottom

        text: "Salva i risultati"

        onClicked: {
            mainLogic.saveAttemptText();
            mainLogic.saveAll();
        }
    }

}
