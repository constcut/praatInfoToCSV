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

    ColumnLayout
    {
        y: 50
        x: 50
        spacing: 30

        RowLayout
        {
            spacing: 10
            Text {
                text: "Audio directory: "
            }
            Text {
                id: audioDirText
            }
            RoundButton {
                text: ".."
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
