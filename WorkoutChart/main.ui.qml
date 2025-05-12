

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
        id: intervalFrame
        x: 50
        height: 450
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 20
        contentHeight: 250

        ComboBox {
            id: workoutTypeComboBox
            anchors.left: parent.left
            anchors.right: removeStep.right
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.rightMargin: 0
            anchors.topMargin: 20
            x: 91
            height: 40
            leftPadding: 20
            model: [qsTr("Absolute Watts"), qsTr("Relative FTP"), qsTr(
                    "Absolute Heart Rate"), qsTr("Percent Max Heart Rate")]
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

        TextField {
            id: workoutName
            height: 40
            anchors.left: removeInterval.right
            anchors.right: parent.right
            anchors.top: workoutTypeComboBox.top
            anchors.leftMargin: 50
            anchors.rightMargin: 20
            anchors.topMargin: 0
            text: workoutChart.workoutName
            placeholderText: "Workout Name"
        }
        TextField {
            id: workoutNotes
            anchors.left: workoutName.left
            anchors.right: parent.right
            anchors.top: addStep.top
            anchors.bottom: repeatSpinBox.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 20
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            wrapMode: Text.WordWrap
            text: workoutChart.workoutNotes
            placeholderText: "Workout Notes"
        }
        SpinBox {
            id: ftpSpinBox
            width: 150
            height: 40
            from: 50
            to: 700
            value: workoutChart.ftp
            anchors.left: workoutTypeComboBox.left
            anchors.top: workoutTypeComboBox.bottom
            anchors.leftMargin: 0
            anchors.topMargin: 50
            Layout.topMargin: 5
            Layout.margins: 50
            Label {
                id: ftpLabel
                text: "FTP:"
                y: -35
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
            }
        }
        SpinBox {
            id: heartRateSpinBox
            width: 150
            height: 40
            from: 60
            to: 220
            value: workoutChart.maxHeartRate
            anchors.left: ftpSpinBox.right
            anchors.top: ftpSpinBox.top
            anchors.leftMargin: 100
            anchors.topMargin: 0
            Label {
                id: heartRateLabel
                y: -35
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
                text: qsTr("Max heart rate:")
            }
        }
        SpinBox {
            id: restingHeartRateSpinBox
            width: 150
            height: 40
            from: 30
            to: 220
            value: workoutChart.restingHeartRate
            anchors.left: workoutName.left
            anchors.top: ftpSpinBox.top
            anchors.leftMargin: 0
            anchors.topMargin: 0
            Label {
                id: restingHeartRateLabel
                y: -35
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
                text: qsTr("Resting heart rate:")
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
            anchors.top: ftpSpinBox.bottom
            anchors.leftMargin: 20
            anchors.topMargin: 50
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
            anchors.left: addIntervalButton.right
            anchors.top: addIntervalButton.bottom
            anchors.leftMargin: 100
            anchors.topMargin: -40
            enabled: workoutChart.removeIntervalEnabled
            Connections {
                target: removeInterval
                function onClicked() {
                    workoutChart.onIntervalRemove()
                }
            }
        }

        SpinBox {
            id: intervalIntensitySpinBox
            width: 150
            height: 40
            value: workoutChart.intensity
            anchors.right: parent.right
            anchors.top: intervalTimeText.top
            anchors.rightMargin: 20
            anchors.topMargin: 0
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
            width: 150
            height: 40
            text: workoutChart.duration
            anchors.left: workoutName.left
            anchors.top: addIntervalButton.top
            anchors.leftMargin: 0
            anchors.topMargin: 0
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
            anchors.top: addIntervalButton.bottom
            anchors.leftMargin: 20
            anchors.topMargin: 50
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
            anchors.left: removeInterval.left
            enabled: workoutChart.removeStepEnabled
            anchors.top: addStep.top
            anchors.leftMargin: 0
            anchors.topMargin: 0
            Connections {
                target: removeStep
                function onClicked() {
                    workoutChart.onStepRemove()
                }
            }
        }
        SpinBox {
            id: fromSpinBox
            width: 150
            height: 40
            anchors.topMargin: 0
            editable: true
            enabled: false
            value: workoutChart.from
            anchors.left: removeStep.left
            anchors.top: repeatSpinBox.top
            anchors.leftMargin: 0

            Label {
                id: fromLabel
                y: -35
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 0
                anchors.topMargin: -20
                text: qsTr("From step nr:")
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
            anchors.top: addStep.bottom
            anchors.leftMargin: 0
            anchors.topMargin: 50
            editable: true
            x: 41
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
    }
    Frame {
        id: workoutFrame
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: intervalFrame.bottom
        anchors.bottom: saveButton.top
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.topMargin: 20
        anchors.bottomMargin: 20

        WorkoutChart {
            id: workoutChart
            maxDuration: 60
            maxIntensity: 500
            // IntervalListModel {
            //     id: intervalList
            //     objectName: "intervalList"
            //     intervalList: [
            //          Interval {
            //             stepList: [
            //                 Step {
            //                     intensity: 200
            //                     duration: 5.0
            //                 }
            //             ]
            //         },
            //         Interval {
            //             repeat: 3
            //             from: 1
            //             stepList: [
            //                 Step {
            //                     intensity: 320
            //                     duration: 5.0
            //                 },
            //                 Step {
            //                     intensity: 50
            //                     duration: 5.0
            //                 }
            //             ]
            //         }
            //     ]
            // }
        }
    }
    FileDialog {
        id: fileDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Fit files (*.fit)", "Plan files (*.plan)", "Erg files (*.erg)", "Mrc files (*.mrc)"]
        defaultSuffix: "fit"
        Connections {
            target: fileDialog
            function onAccepted() {
                workoutChart.fileName = decodeURI(fileDialog.selectedFile)
            }
        }
    }
    RoundButton {
        id: saveButton
        width: 150
        height: 40
        radius: 10
        text: qsTr("&Save As...")
        anchors.right: cancelButton.left
        anchors.bottom: parent.bottom
        anchors.rightMargin: 100
        anchors.bottomMargin: 20
        highlighted: true
        font.bold: false
        autoExclusive: false
        Connections {
            target: saveButton
            function onClicked() {
                fileDialog.open()
            }
        }
    }
    RoundButton {
        id: cancelButton
        width: 150
        height: 40
        radius: 10
        text: qsTr("E&xit")
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
}
