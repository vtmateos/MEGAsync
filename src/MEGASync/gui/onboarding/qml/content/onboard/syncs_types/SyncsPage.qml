import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.0
import Common 1.0

Rectangle {
    id: mainItem

    property alias footerButtons: footerButtons
    anchors.fill: parent
    color: Styles.backgroundColor

    Footer {
        id: footerButtons

        anchors{
            bottom: parent.bottom
            right: parent.right
            rightMargin: 20
            bottomMargin: 20
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
