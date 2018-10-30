import QtQuick 2.2
import QtQuick.Controls 1.5
import QtQml.Models 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.1

import SettingsTreeModel 1.0
import SettingsTreeItem 1.0

ApplicationWindow {
    visible: true
    width: 1080
    height: 720
    title: qsTr("File System")

    minimumWidth: 640
    minimumHeight: 480

    FileDialog {
        id: fileDialog
        folder: shortcuts.home

        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
        }
        onRejected: {

        }
        Component.onCompleted: visible = false
    }

    MessageDialog  {
        id: messageDialog
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle {
            id: settingsViewRect
            width: parent.width * 0.4
            Layout.minimumWidth: parent.width * 0.4
            color: "white"

            Column {
                anchors.fill: parent
                anchors.horizontalCenter: parent.horizontalCenter

                TreeView {
                    id: view
                    width: parent.width
                    height: parent.height * 0.9
                    model: SettingsTreeModel
                    rootIndex: ModelRootIndex

                    TableViewColumn {
                        title: "Settings"
                        role: "NameRole"
                        resizable: true
                    }

                    property real _val: model.data(model.index(0,0), SettingsTreeRoles.ValRole)

                    selection: ItemSelectionModel {
                        model: SettingsTreeModel
                    }

                    style: TreeViewStyle {
                        id: treeStyle
                        rowDelegate: Rectangle {
                            color: "white"
                            height: 30
                            gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#FFFFFF" }
                                    GradientStop { position: 1.0; color: "#effeff" }
                                }
                        }
                        itemDelegate: Item {
                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                color: styleData.selected ? "#128fbb" : styleData.textColor
                                elide: styleData.elideMode
                                text: styleData.value
                                font.pixelSize: Qt.application.font.pixelSize * 1.3
                            }
                        }
                        branchDelegate: Rectangle {
                            width: 15
                            Text {
                                text: styleData.isExpanded ? "\u25BC" : "\u25B6"
                                color: styleData.selected ? "#128fbb" : "grey"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pixelSize: Qt.application.font.pixelSize * (styleData.isExpanded ? 1 : 1.5)
                            }
                        }

                        activateItemOnSingleClick: true
                    }

                    onActivated: {
                        if(view.isExpanded(index)) view.collapse(index)
                        else view.expand(index)
                        view.selection.setCurrentIndex(index, ItemSelectionModel.Select)
                        _val = model.data(selection.currentIndex, SettingsTreeRoles.ValRole)
                        console.log(_val)
                    }

                    Component.onCompleted: {
                        view.selection.setCurrentIndex(view.model.index(0,0), ItemSelectionModel.Select)
                    }
                }

                Rectangle {
                    width: parent.width
                    height: parent.height * 0.1

                    Row {
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 10

                        Repeater {
                            id: saveSettings
                            model: ["Reset", "Save", "Save As ..."]
                            Button {
                                width: settingsViewRect.width / (saveSettings.count + 1)
                                text: modelData
                                anchors.verticalCenter: parent.verticalCenter

                                onClicked: {
                                    if(index === 0){
                                        SettingsTreeModel.saveSettingsFile(true);
                                    }
                                    else if(index === 1){
                                        SettingsTreeModel.saveSettingsFile();
                                    }
                                    else if(index === 2){
                                        fileDialog.title = "Save settings as ..."
                                        fileDialog.selectExisting = false
                                        fileDialog.open()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: sidePanel
            width: parent.width * 0.6
            Layout.minimumWidth: parent.width * 0.4

            Column {
                anchors.fill: parent
                anchors.verticalCenter: parent.verticalCenter
                spacing: 5

                Rectangle {
                    id: settingsWidget
                    width: parent.width
                    height: sidePanel.height * 0.2
                    anchors.horizontalCenter: parent.horizontalCenter

                    Row {
                        id: widgetRow
                        height: parent.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 20

                        property real _type: view.model.data(view.selection.currentIndex, SettingsTreeRoles.TypeRole)
                        property real _defV: view.model.data(view.selection.currentIndex, SettingsTreeRoles.DefValRole)
                        property real _minV: view.model.data(view.selection.currentIndex, SettingsTreeRoles.MinValRole)
                        property real _maxV: view.model.data(view.selection.currentIndex, SettingsTreeRoles.MaxValRole)
                        property real _step: view.model.data(view.selection.currentIndex, SettingsTreeRoles.StepSizeRole)

                        Text {
                            id: settingsTitle
                            text: view.model.data(view.selection.currentIndex, SettingsTreeRoles.NameRole)
                            visible: widgetRow._type ? widgetRow._type : false
                            anchors.verticalCenter: parent.verticalCenter
                            width: settingsWidget.width * 0.2
                        }

                        Slider {
                            id: widgetRowSlider
                            anchors.verticalCenter: parent.verticalCenter

                            visible: widgetRow._type === WidgetType.Slider
                            minimumValue: widgetRow._minV
                            maximumValue: widgetRow._maxV
                            stepSize: widgetRow._step
                            value: view._val

                            style: SliderStyle {
                                groove: Rectangle {
                                    implicitWidth: settingsWidget.width * 0.3
                                    implicitHeight: 8
                                    color: "gray"
                                    radius: 8
                                }
                                handle: Rectangle {
                                    anchors.centerIn: parent
                                    color: control.pressed ? "white" : "lightgray"
                                    border.color: "gray"
                                    border.width: 2
                                    implicitWidth: 15
                                    implicitHeight: 15
                                    radius: 12
                                }
                            }
                        }

                        Text {
                            id: sliderValueText
                            text: widgetRowSlider.value.toFixed(2)
                            visible: widgetRowSlider.visible
                            anchors.verticalCenter: parent.verticalCenter
                            width: settingsWidget.width * 0.1
                        }

                        SpinBox {
                            id: widgetRowSpinBox
                            anchors.verticalCenter: parent.verticalCenter

                            visible: widgetRow._type === WidgetType.Spinner
                            minimumValue: widgetRow._minV
                            maximumValue: widgetRow._maxV
                            stepSize: widgetRow._step
                            value: view._val

                            style: SpinBoxStyle{
                                background: Rectangle {
                                    implicitWidth: settingsWidget.width * 0.3
                                    implicitHeight: 20
                                    border.color: "gray"
                                    radius: 2
                                }
                            }
                        }

                        Text {
                            id: spinBoxSpaceItem
                            text: ""
                            width: settingsWidget.width * 0.1
                            visible: widgetRowSpinBox.visible
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        CheckBox {
                            id: widgetCheckBox
                            visible: widgetRow._type === WidgetType.CheckBox
                            anchors.verticalCenter: parent.verticalCenter
                            width: settingsWidget.width * 0.1
                            checked: view._val
                        }

                        Text {
                            id: checkBoxSpaceItem
                            text: ""
                            width: settingsWidget.width * 0.3
                            visible: widgetCheckBox.visible
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Column {
                            width: settingsWidget.width * 0.2
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 10

                            Button {
                                id: setValue
                                width: parent.width
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Set"
                                visible: widgetRow._type ? widgetRow._type : false

                                onClicked: {
                                    switch(widgetRow._type){
                                    case WidgetType.Slider:
                                        view.model.setData(view.selection.currentIndex, widgetRowSlider.value, SettingsTreeRoles.ValRole)
                                        break
                                    case WidgetType.Spinner:
                                        view.model.setData(view.selection.currentIndex, widgetRowSpinBox.value, SettingsTreeRoles.ValRole)
                                        break
                                    }
                                }
                            }

                            Button {
                                id: setDefault
                                width: parent.width
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Set to Default"
                                visible: widgetRow._type ? widgetRow._type : false

                                onClicked: {
                                    view.model.setData(view.selection.currentIndex, widgetRow._defV, SettingsTreeRoles.ValRole)
                                    view._val = widgetRow._defV
                                }
                            }
                        }
                    }
                }

                TextArea {
                    id: settingsDescription
                    width: parent.width
                    height: sidePanel.height * 0.8
                    anchors.horizontalCenter: parent.horizontalCenter

                    readOnly: true
                    property string _desc: view.model.data(view.selection.currentIndex, SettingsTreeRoles.DescriptionRole)
                    text: _desc ? _desc : "Default description"
                }
            }
        }
    }
}
