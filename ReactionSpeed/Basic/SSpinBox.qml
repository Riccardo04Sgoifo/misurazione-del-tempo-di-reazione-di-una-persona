import QtQuick 2.15
import QtQuick.Controls.Fusion

SpinBox {

    editable: true

    property string timeMode: "MILLISECONDS"

    from: 0
    to: 10000

    textFromValue: function (value) {
        return Number(Math.round(Number(parseFloat(value) * ((timeMode === "SECONDS") ? 1.0 / 1000.0 : 1.0)) * 1000.0) / 1000.0).toString() + ((timeMode === "SECONDS") ? " s" : "ms");
    }


    valueFromText: function (value) {

        let toCheck = value.toLowerCase();

        if (toCheck.includes("s") && (!toCheck.includes("ms"))) {
            timeMode = "SECONDS";

        }
        else{
            timeMode = "MILLISECONDS";

        }

        return parseFloat(value) * ((timeMode === "SECONDS") ? 1000 : 1);
    }
}
