import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import Onboard.Syncs_types 1.0
import Common 1.0

SyncsPage {

    property alias buttonGroup: buttonGroup

    ColumnLayout {
        spacing: 34
        anchors.left: parent.left
        anchors.leftMargin: 32

        Header {
            title: qsTr("Choose sync type")
            description: qsTr("Lorem ipsum dolor asitmet")
            Layout.fillWidth: false
            Layout.preferredWidth: 488
            Layout.topMargin: 32
        }

        Rectangle {
            Layout.preferredHeight: 208
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignLeft
            color: Styles.surface1

            ButtonGroup {
                id: buttonGroup
            }

            RowLayout {
                spacing: 8
                anchors.fill: parent

                ResumeButton {
                    id: fullSyncButton

                    title: qsTr("Full Sync")
                    type: ResumeButton.Type.FullSync
                    description: qsTr("Sync your entire Cloud Drive.")
                    imageSource: "../../../../../images/Onboarding/syncs/full_sync.svg"
                    ButtonGroup.group: buttonGroup
                    imageSourceSize: Qt.size(200, 96)
                }

                ResumeButton {
                    id: backupsButton

                    title: qsTr("Selective Sync")
                    type: ResumeButton.Type.SelectiveSync
                    description: qsTr("Sync specific folders in your Cloud Drive.")
                    imageSource: "../../../../../images/Onboarding/syncs/selective_sync.svg"
                    ButtonGroup.group: buttonGroup
                    imageSourceSize: Qt.size(200, 96)
                }
            }
        }
    }
}
