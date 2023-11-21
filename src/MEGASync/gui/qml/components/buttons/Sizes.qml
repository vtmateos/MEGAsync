import QtQuick 2.15

import components.texts 1.0

QtObject {

    // Medium sizes
    property int horizontalPadding: 16
    property int verticalPadding: horizontalPadding/2
    property int height: 36
    property int spacing: 8
    property int radius: 6
    property int maskBorderRadius: 8
    property int focusBorderRadius: 10
    property int focusBorderWidth: 4
    property int borderWidth: 2
    property int iconWidth: 16
    property size iconSize: Qt.size(iconWidth, iconWidth)
    property int textFontSize: Text.Size.Medium

}
