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

    function updateStatistics() {
        meanTextField.relaod();
        errorTextField.relaod();
        bestTimeTextField.relaod();
        topPTextField.relaod();
    }

    ScrollView {
        id: scrollView

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: statisticsPanel.left

        contentHeight: textEdit.height


        STextEdit {

            id: textEdit

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            text: mainLogic.scoreBoardText

            height: contentHeight

            onTextChanged: {

                mainLogic.setScoreBoardText(text);

                if (!focus){
                    scrollView.ScrollBar.vertical.position = 1.0 - scrollView.ScrollBar.vertical.size;
                }
                else {
                    console.log(cursorRectangle.y );
                    console.log(contentHeight);
                    console.log(scrollView.ScrollBar.vertical.visualPosition)

                    if (cursorRectangle.y + cursorRectangle.height > contentHeight * scrollView.ScrollBar.vertical.visualPosition + scrollView.height){
                        scrollView.ScrollBar.vertical.position = Math.min((cursorRectangle.y) / contentHeight, 1.0 - scrollView.ScrollBar.vertical.size)
                    }


                }
            }

            readOnly: false
            wrapMode: TextEdit.Wrap

            selectByMouse: true


        }
    }

    Rectangle {
        id: statisticsPanel

        width: 300

        //anchors.left: scrollView.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        radius: 10

        color: "dark gray"

        // Mean

        RowLayout {

            id: meanDisplay
            anchors.margins: 5

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {

                id: meanTextField

                anchors.right: meanReloadButton.left
                anchors.left: parent.left
                anchors.margins: 5

                text: "Media: 234ms"

                property int mean: 0

                function reload() {
                    var lines = textEdit.text.split("\n");

                    console.log(lines);

                    var sum = 0;
                    var count = 0;

                    for (var i = 1; i < lines.length; i++){ // first line is name
                        if (lines[i] !== ""){
                            if (lines[i].split(":").length > 1){
                                var value = parseInt(lines[i].split(":")[1].replace(/\D/g, ''));
                                if (value !== NaN){
                                    sum += value;
                                    count ++;
                                }
                            }

                        }
                    }
                    mean = (Math.floor(sum / count));

                    text = "Media: " + String(Math.floor(sum / count)) + "ms";
                }

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: "Media calcolata a partire dalla riga 2"


            }


            SReloadButton {
                id: meanReloadButton
                anchors.right: parent.right
                anchors.margins: 5

                target: meanTextField
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

                id: errorTextField

                anchors.right: errorReloadButton.left
                anchors.left: parent.left
                anchors.margins: 5

                text: "Errori: 3"

                function reload() {
                    var lines = textEdit.text.split("\n");

                    var sum = 0;
                    var count = 0;
                    var threshold = 1.5

                    for (var i = 1; i < lines.length; i++){ // first line is name
                        if (lines[i] !== ""){
                            if (lines[i].split(":").length > 1){
                                var value = parseInt(lines[i].split(":")[1].replace(/\D/g, ''));
                                if (value !== NaN){
                                    if (value >= meanTextField.mean * threshold){
                                        count ++;
                                    }
                                }

                            }

                        }
                    }

                    text = "Errori: " + String(Math.floor(count));
                }


                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: "tempi sopra la soglia di 1.5 volte la media"
            }

            SReloadButton {
                target: errorTextField

                id: errorReloadButton
                anchors.right: parent.right
                anchors.margins: 5
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

                id: bestTimeTextField

                anchors.right: bestTimeReloadButton.left
                anchors.left: parent.left
                anchors.margins: 5

                text: "Tempo migliore: 125ms"

                function reload() {
                    var lines = textEdit.text.split("\n");

                    var best = Infinity;

                    for (var i = 1; i < lines.length; i++){ // first line is name
                        if (lines[i] !== ""){
                            if (lines[i].split(":").length > 1){
                                var value = parseInt(lines[i].split(":")[1].replace(/\D/g, ''));
                                if (value !== NaN){
                                    if (value < best){
                                        best = value;
                                    }
                                }
                            }

                        }
                    }

                    text = "Migliore: " + String(Math.floor(best)) + "ms";
                }

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: "tempo più basso"


            }

            SReloadButton {
                target: bestTimeTextField

                id: bestTimeReloadButton
                anchors.right: parent.right
                anchors.margins: 5
            }

        }

        // Top percentage

        /*RowLayout {

            id: topPDisplay
            anchors.margins: 5

            anchors.top: bestTimeDisplay.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            TextField {

                id: topPTextField

                anchors.right: topPReloadButton.left
                anchors.left: parent.left
                anchors.margins: 5

                text: "Percentuale migliore: 51%"

                function reload() {


                    var scores = mainLogic.getMeans();

                    if (scores.length < 1){
                        text = "Top P: 100%";
                    }
                    else {
                        var below = 0

                        var mean = meanTextField.mean

                        for (let i = 0; i < scores.length; i++){
                            if (scores[i] < mean) {
                                below ++;
                            }
                        }

                        var percentile = 1.0 - below / scores.length;

                        text = "Top P: " + String(Math.floor(percentile * 100.0)) + "%";
                    }


                }

                ToolTip.visible: hovered
                ToolTip.delay: 500
                ToolTip.text: "percentuale di tempi migliori (media)"


            }

            SReloadButton {
                target: topPTextField

                id: topPReloadButton
                anchors.right: parent.right
                anchors.margins: 5
            }

        }*/

    }
}
