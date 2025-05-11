

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import WorkoutChart 1.0

Rectangle {
    id: parentRectangle
    width: 1025
    height: 768
    anchors.fill: parent
    color: "#eaeaea"
    Frame {
        id: fileFrame
        x: 50
        y: 50
        height: 180
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 20
        contentHeight: 130

        ComboBox {
            id: fileTypeComboBox
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            anchors.topMargin: 50
            currentIndex: workoutChart.fileType
            property int fileType: 0
            x: 91
            y: 91
            leftPadding: 20
            model: [qsTr("Fit file"), qsTr("Plan file"), qsTr("Erg file"), qsTr(
                    "Mrc file")]

            Label {
                id: fileLabel
                x: -1240
                y: -30
                text: qsTr("File type:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -30
            }

            Connections {
                target: fileTypeComboBox
                function onActivated() {
                    workoutChart.fileType = fileTypeComboBox.currentIndex
                        switch (fileTypeComboBox.currentIndex) {
                            case 0: // Fit
                                fileDialog.nameFilters = ["Fit files (*.fit)"];
                                fileDialog.defaultSuffix = "fit";
                                break;
                            case 1: // Plan
                                fileDialog.nameFilters = ["Plan files (*.plan)"];
                                fileDialog.defaultSuffix = "plan";
                                break;
                            case 2: // Erg
                                fileDialog.nameFilters = ["Erg files (*.erg)"];
                                fileDialog.defaultSuffix = "erg";
                                break;
                            case 3: // Mrc
                                fileDialog.nameFilters = ["Mrc files (*.mrc)"];
                                fileDialog.defaultSuffix = "mrc";
                                break;
                    }
                }
            }
        }

        TextField {
            id: fileNameText
            height: 40
            anchors.left: fileTypeComboBox.left
            anchors.right: fileDialogButton.left
            anchors.top: fileTypeComboBox.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 20
            anchors.topMargin: 20
            placeholderText: qsTr("File name")
            text: workoutChart.fileName
        }

        FileDialog {
            id: fileDialog
            fileMode: FileDialog.SaveFile
            nameFilters: ["Fit files (*.fit)"];
            defaultSuffix : "fit"
            Connections {
                target: fileDialog
                function onAccepted() {
                    workoutChart.fileName = decodeURI(fileDialog.selectedFile)
                }
            }
        }
        Button {
            id: fileDialogButton
            width: 40
            height: 40
            text: "..."
            anchors.right: parent.right
            anchors.top: fileTypeComboBox.bottom
            anchors.rightMargin: 20
            anchors.topMargin: 20
            bottomPadding: 0
            rightPadding: 0
            leftPadding: 0
            topPadding: 0

            Connections {
                target: fileDialogButton
                function onClicked() {
                    fileDialog.open()
                }
            }
        }
    }
    Frame {
        id: intervalFrame
        x: 50
        height: 300
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: fileFrame.bottom
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 20
        contentHeight: 250

        ComboBox {
            id: workoutTypeComboBox
            anchors.left: parent.left
            anchors.right: removeInterval.left
            anchors.top: addIntervalButton.bottom
            anchors.leftMargin: 20
            anchors.rightMargin: 50
            anchors.topMargin: 30
            x: 91
            height: 40
            leftPadding: 20
            model: [qsTr("Absolute Watts"), qsTr("Relative FTP"), qsTr(
                    "Absolute Heart Rate"), qsTr("Percent Max Heart Rate")
                ]
            currentIndex: workoutChart.workoutType

            Label {
                id: workoutTypeLabel
                y: 110
                text: qsTr("Workout type:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
            }

            Connections {
                target: workoutTypeComboBox
                function onActivated() {
                    workoutChart.workoutType = workoutTypeComboBox.currentIndex
                }
            }
        }

        SpinBox {
            id: fromSpinBox
            width: 150
            height: 40
            anchors.topMargin: 30
            editable: true
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: false
            value: workoutChart.from
            y: 221

            Label {
                id: fromLabel
                y: -35
                text: qsTr("From step nr:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
            }
            from: 1
            Connections {
                target: fromSpinBox
                function onValueModified() {
                    workoutChart.from = fromSpinBox.value
                }
            }
        }

        SpinBox {
            id: repeatSpinBox
            width: 150
            height: 40
            value: workoutChart.repeats
            anchors.left: workoutTypeComboBox.left
            anchors.leftMargin: 0
            anchors.topMargin: 30
            editable: true
            x: 41
            y: 221
            from: 1

            Label {
                id: repeatLabel
                y: -174
                text: qsTr("Repeat steps times:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
            }
            Connections {
                target: repeatSpinBox
                function onValueModified() {
                    if (repeatSpinBox.value > 0) {
                        fromSpinBox.enabled = true
                        fromSpinBox.to = workoutChart.maxFrom
                    } else {
                        fromSpinBox.enabled = false
                    }
                    workoutChart.repeats = repeatSpinBox.value
                }
            }
        }

        RoundButton {
            id: addIntervalButton
            x: 141
            y: 271
            width: 150
            height: 40
            radius: 10
            enabled: workoutChart.addIntervalEnabled
            text: qsTr("&Add interval")
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.topMargin: 10
            Connections {
                target: addIntervalButton
                function onClicked() {
                    workoutChart.onIntervalAdd()
                }
            }
        }

        RoundButton {
            id: removeInterval
            objectName: "removeInterval"
            y: 271
            width: 150
            height: 40
            radius: 10
            text: qsTr("&Remove interval")
            anchors.left: parent.horizontalCenter
            anchors.top: parent.top
            anchors.leftMargin: -75
            anchors.topMargin: 10
            enabled: workoutChart.removeIntervalEnabled
            Connections {
                target: removeInterval
                function onClicked() {
                    workoutChart.onIntervalRemove()
                }
            }
        }

        Dialog {
            id: settingDialog
            title: qsTr("Settings")
            standardButtons: Dialog.Ok | Dialog.Cancel
            ColumnLayout {
                Label {
                    id: ftpLabel
                    text: "FTP:"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
                SpinBox {
                    id: ftpSpinBox
                    from: 50
                    to: 700
                    value: workoutChart.ftp
                    Layout.topMargin: 5
                    Layout.margins: 50
                }
                Label {
                    id: heartRateLabel
                    text: qsTr("Max heart rate:")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
                SpinBox {
                    id: heartRateSpinBox
                    Layout.topMargin: 5
                    Layout.margins: 50
                    from: 60
                    to: 220
                    value: workoutChart.maxHeartRate
                }
            }
        }
        Button {
            id: settingsButton
            y: 271
            width: 40
            height: 40
            text: "..."
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 20
            anchors.topMargin: 10
            bottomPadding: 0
            rightPadding: 0
            leftPadding: 0
            topPadding: 0
            Connections {
                target: settingsButton
                function onClicked() {
                    settingDialog.open()
                }
            }
        }

        SpinBox {
            id: intervalIntensitySpinBox
            objectName: "intervalValue"
            y: 221
            width: 150
            height: 40
            value: workoutChart.intensity
            anchors.right: parent.right
            anchors.top: addIntervalButton.bottom
            anchors.rightMargin: 20
            anchors.topMargin: 30
            editable: true
            stepSize: 25
            enabled: true
            Label {
                id: fromLabel1
                y: -35
                text: qsTr("Intensity")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
            }
            to: 2000
            from: 1
            Connections {
                target: intervalIntensitySpinBox
                function onValueModified() {
                    workoutChart.intensity = intervalIntensitySpinBox.value
                }
            }
        }

        TextField {
            id: intervalTimeText
            text: workoutChart.duration
            anchors.left: removeInterval.left
            anchors.right: removeInterval.right
            anchors.top: removeInterval.bottom
            anchors.bottom: workoutTypeComboBox.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.topMargin: 30
            anchors.bottomMargin: 0
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: true
            placeholderTextColor: "#78000000"
            placeholderText: "mm:ss"
            inputMask: "99:99"
            validator: RegularExpressionValidator {
                regularExpression: /^([0-5][0-9]):([0-5][0-9])$/
            }
            Label {
                id: intervalTimeLabel
                text: qsTr("Duration:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
            }

            Connections {
                target: intervalTimeText
                function onEditingFinished() { 
                    workoutChart.duration = intervalTimeText.text 
                }
            }
        }

        RoundButton {
            id: addStep
            width: 150
            height: 40
            radius: 10
            enabled: workoutChart.addStepEnabled
            text: qsTr("Add &step")
            anchors.left: parent.left
            anchors.top: workoutTypeComboBox.bottom
            anchors.leftMargin: 20
            anchors.topMargin: 20
            Connections {
                target: addStep
                function onClicked() {
                    workoutChart.onStepAdd()
                }
            }
        }
        RoundButton {
            id: removeStep
            width: 150
            height: 40
            radius: 10
            text: qsTr("R&emove step")
            enabled: workoutChart.removeStepEnabled
            anchors.top: workoutTypeComboBox.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: removeInterval.horizontalCenter
            Connections {
                target: removeStep
                function onClicked() {
                    workoutChart.onStepRemove()
                }
            }
        }
    }
    Frame {
        id: workoutFrame
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: intervalFrame.bottom
        anchors.bottom: okButton.top
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 20
        anchors.bottomMargin: 20

        WorkoutChart {
            id: workoutChart
            anchors.fill: parent
            maxDuration: 60
            maxIntensity: 500
            IntervalListModel {
                id: intervalList
                objectName: "intervalList"
                intervalList: [
/*                     Interval {
                        stepList: [
                            Step {
                                intensity: 200
                                duration: 5.0
                            }
                        ]
                    },
                    Interval {
                        repeat: 3
                        from: 1
                        stepList: [
                            Step {
                                intensity: 320
                                duration: 5.0
                            },
                            Step {
                                intensity: 50
                                duration: 5.0
                            }
                        ]
                    } */
                ]
            }
        }
    }
    RoundButton {
        id: okButton
        width: 150
        height: 40
        radius: 10
        text: qsTr("&Ok")
        anchors.right: cancelButton.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 100
        anchors.bottomMargin: 20
        highlighted: true
        font.bold: false
        autoExclusive: false
        Connections {
            target: okButton
            function onClicked() {
                workoutChart.onOkClicked()
            }
        }
    }
    RoundButton {
        id: cancelButton
        width: 150
        height: 40
        radius: 10
        text: qsTr("&Cancel")
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 100
        anchors.bottomMargin: 20
        Connections {
            target: cancelButton
            function onClicked() {
                Qt.quit()
            }
        }
    }

    Connections {
        id: connections
        target: settingDialog
        function onAccepted() {
            workoutChart.ftp = ftpSpinBox.value
            workoutChart.maxHeartRate = heartRateSpinBox.value
        }
    }
}
