import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Layouts

Button {

    implicitWidth: height

    text: ""
    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

    property var target;

    onClicked: {
        target.reload();
    }

    Image {

        anchors.centerIn: parent
        source: "qrc:///images/reload.png"
        fillMode: Image.PreserveAspectFit
        sourceSize.height: parent.background.height - 6
        height: sourceSize.height
        verticalAlignment: Image.AlignVCenter
    }


}
