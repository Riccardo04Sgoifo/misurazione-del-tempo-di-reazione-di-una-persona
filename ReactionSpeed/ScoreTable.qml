import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import Basic

Item {
    Layout.alignment: Qt.AlignTop
    Layout.fillWidth: true
    Layout.minimumHeight: 200

    STextEdit {
        anchors.fill: parent
        text: "scores: \nhere times will be displayed"

        readOnly: false
        wrapMode: TextEdit.Wrap
    }
}
