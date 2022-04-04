import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1


Window
{
    width: 1024
    height: 480
    visible: true
    title: qsTr("Praat info to CSV")



    FileDialog
    {
        id: saveFileDialog
        title: "Save csv to file"
        folder: shortcuts.desktop

        selectExisting: false
        selectMultiple: false

        onAccepted: {
            var filename = saveFileDialog.fileUrls[0].substring(8)
            outputFileText.text = filename
            saveFileDialog.visible = false
        }
        onRejected: {
            saveFileDialog.visible = false
        }
        nameFilters: [ "CSV (*.csv)" ]
    }

    FileDialog
    {
        id: directoryDialog
        title: "Please choose praat info directory"
        folder: shortcuts.desktop

        selectExisting: true
        selectMultiple: false
        selectFolder: true

        onAccepted: {
            var dirName = directoryDialog.fileUrls[0].substring(8)
            infoDirText.text = dirName
            directoryDialog.visible = false
        }
        onRejected: {
            directoryDialog.visible = false
        }
    }


    Connections {

        id: connector
        target: watchDog

        function notifyFilled(list)
        {
            for (var i = 0; i < list.length; ++i)
            {
                fieldsRepeater.itemAt(i).text = list[i]
            }
        }
    }

    ColumnLayout
    {
        y: 50
        x: 50
        spacing: 30

        RowLayout
        {
            spacing: 10
            Text {
                text: "Info directory: "
            }
            Text {
                id: infoDirText
            }
            RoundButton {
                text: ".."
                onClicked: directoryDialog.open()
            }
        }

        RowLayout
        {
            spacing: 10
            Text {
                text: "Output file: "
            }
            Text {
                id: outputFileText
            }
            RoundButton {
                text: ".."
                onClicked: saveFileDialog.open()
            }
        }

        RowLayout
        {
            spacing: 20

            Text {
                text: "Date: "
            }
            Text {
                id: dateText
            }
            RoundButton {
                text: ".."
                onClicked: {
                    calendarDialog.open()
                }
            }
            Text {
                text: "Time:"
            }
            ComboBox {
                id: timeCombo
                model: ["Утро", "Вечер"]
            }
            Text {
                text: "Name: "
            }
            TextField {
                id: nameField
                placeholderText: "Name"
            }

            RoundButton {
                text: "Hide"
                onClicked:  {
                    notifyRepeater.hideAll()
                }
            }

            RoundButton {
                text: "Show"
                onClicked:  {
                    notifyRepeater.cloneAll()
                    notifyRepeater.showAll()
                }
            }

        }

        RowLayout
        {
            spacing:  10

            Repeater
            {
                id: fieldsRepeater
                model : 5

                TextField {
                    id: localField
                    placeholderText: index
                    implicitWidth: 100
                }
            }
        }


        RowLayout
        {
            spacing:  10



            Repeater
            {
                id: notifyRepeater
                model : 5

                function showAll()
                {
                    for (var i = 0; i < model; ++i)
                        notifyRepeater.itemAt(i).showField()
                }

                function cloneAll() {
                    for (var i = 0; i < model; ++i)
                        notifyRepeater.itemAt(i).text
                            = fieldsRepeater.itemAt(i).text
                }

                function hideAll()
                {
                    for (var i = 0; i < model; ++i)
                        notifyRepeater.itemAt(i).startAnimation()
                }

                TextField
                {
                    id: notifyField
                    placeholderText: index
                    implicitWidth: 100

                    function startAnimation() {
                        opacityAnimation.start()
                    }

                    function showField() {
                        notifyField.opacity = 1.0
                    }

                    NumberAnimation
                    {
                        id: opacityAnimation
                        target: notifyField
                        property: "opacity"
                        duration: 1500
                        from: 1.0
                        to: 0.0
                    }
                }
            }
        }

    }


    Dialog
    {
        id: calendarDialog

        width: 400
        height: 400

        Quick1.Calendar
        {
            anchors.fill: parent
            id: calendar

            onSelectedDateChanged: {
                dateText.text = selectedDate
                dateText.text = dateText.text.substring(0, 10)
            }

            Component.onCompleted: {
                dateText.text = selectedDate
                dateText.text = dateText.text.substring(0, 10)
            }
        }
    }


}
