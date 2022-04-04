import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4 as Quick1


Window {


    width: 1024
    height: 480
    visible: true
    title: qsTr("Praat info to CSV")

    ColumnLayout
    {
        y: 50
        x: 50
        spacing: 10

        RowLayout
        {

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
