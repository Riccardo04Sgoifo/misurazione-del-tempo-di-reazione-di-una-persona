qt_add_library(BasicLib STATIC)

set_target_properties(BasicLib PROPERTIES AUTOMOC ON)
target_link_libraries(BasicLib PRIVATE Qt6::Quick)

list (
    APPEND MODULE_QML_FILES
    SText.qml
    SSpinBox.qml
    SCheckBox.qml
    STextEdit.qml
    SReloadButton.qml
)

qt_add_qml_module(BasicLib
    URI Basic
    VERSION 1.0
    QML_FILES
        ${MODULE_QML_FILES}
)

