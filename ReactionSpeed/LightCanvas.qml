import QtQuick
import QtQuick.Effects


Rectangle {

    id: canvas

    property color newColor: Qt.rgba(1, 0, 0, 1)
    property color circleColor: Qt.rgba(1, 0, 0, 1)
    property color disabledColor: Qt.rgba(0.176, 0.161, 0.149, 1.00)


    property int lightNum: mainLogic.lightNum
    property int currentLight: mainLogic.currentLight



    ColorAnimation on circleColor {
        id: colorAnimation;
        running: false;
        duration: 300;
        from: canvas.disabledColor;
        to: canvas.circleColor;
    }

    onCircleColorChanged: {
        drawingCanvas.requestPaint();
    }

    onCurrentLightChanged: {
        colorAnimation.restart();
        drawingCanvas.requestPaint();
    }

    Canvas {

        id: drawingCanvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");

            var separation = Math.floor(canvas.width / (canvas.lightNum + 1));

            var radius = 50;

            var height = Math.floor(canvas.height / 2) - radius;


            for (let i = 0; i < canvas.lightNum; i++){
                ctx.beginPath();
                ctx.fillStyle = (i == canvas.currentLight) ? circleColor : disabledColor;
                ctx.ellipse(10 + (i + 1) * separation - radius, height, radius * 2, radius * 2);
                ctx.fill();
            }

        }

    }

    ShaderEffect {
        anchors.fill: parent

        property var u_resolution: Qt.vector2d(parent.width, parent.height);

        property int num: canvas.lightNum;
        property int currentLight: canvas.currentLight;

        fragmentShader: "/lights.frag.qsb"
    }
}
