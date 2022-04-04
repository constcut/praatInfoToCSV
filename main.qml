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

            watchDog.setCSVfilename(filename)
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

            watchDog.setInfoDirectory(dirName)
        }
        onRejected: {
            directoryDialog.visible = false
        }
    }


    Connections {

        id: connector
        target: watchDog

        function onNotifyFilled(list)
        {
            for (var i = 0; i < list.length; ++i)
                fieldsRepeater.itemAt(i + 1).text = list[i]
        }

        function onNotifyLastFilled(last)
        {
            fieldsRepeater.itemAt(0).text = last
            notifyRepeater.showAll()
            notifyRepeater.cloneAll()
            notifyRepeater.hideAll()

            if (dayTimeCombo.currentIndex == 0)
            {
                dayTimeCombo.currentIndex = 1
            }
            else {
                dayTimeCombo.currentIndex = 0
                var theDay = calendar.selectedDate
                theDay.setDate(theDay.getDate() + 1);
                calendar.selectedDate = theDay
            }

            watchDog.setDate(dateText.text)
            watchDog.setDayTime(dayTimeCombo.currentText)
        }
    }

    Component.onCompleted: {
        notifyRepeater.model = watchDog.totalFields()
        fieldsRepeater.model = watchDog.totalFields()
        notifyRepeater.hideAll()
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
                id: dayTimeCombo
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
                text: "Set"
                onClicked: {
                    watchDog.setDate(dateText.text)
                    watchDog.setName(nameField.text)
                    watchDog.setDayTime(dayTimeCombo.currentText)
                }
            }

        }

        RowLayout
        {
            spacing:  10

            Repeater
            {
                id: fieldsRepeater
                model : 7

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
                model : 7

                function showAll()
                {
                    for (var i = 0; i < model; ++i)
                        notifyRepeater.itemAt(i).showField()
                }

                function cloneAll() {
                    for (var i = 0; i < model; ++i)
                    {
                        notifyRepeater.itemAt(i).text = fieldsRepeater.itemAt(i).text
                        fieldsRepeater.itemAt(i).text = ""
                    }
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
                        duration: 3500
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
