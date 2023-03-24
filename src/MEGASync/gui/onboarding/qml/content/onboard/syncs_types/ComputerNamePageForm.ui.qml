import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.0
import Components 1.0 as Custom

SyncsPage {

    objectName: "ComputerNamePageForm"

    ColumnLayout {

        spacing: 12

        Header {
            title: qsTr("Set up MEGA")
            description: qsTr("You can assign the name for personal use or workgroup membership of this computer.")
            Layout.fillWidth: false
            Layout.preferredWidth: 488
            Layout.topMargin: 32
            Layout.leftMargin: 32
        }

        ComputerName {
            Layout.fillWidth: false
            Layout.preferredWidth: 488
            Layout.leftMargin: 32
        }
    }
}
