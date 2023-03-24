import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12

import Components 1.0
import Onboarding 1.0
import com.qmldialog 1.0 as Cpp
import Onboard.Syncs_types 1.0

Cpp.QmlDialog {
    id: mWindow

    objectName: "app1"
    title: "Set up MEGA"
    modality: Qt.NonModal
    width: 776
    height: 544

    SyncsFlow
    /*OnboardingFlow*/ {
        id: onboarding

        anchors.fill: parent
    }

    Connections {
        target: Onboarding

        onNotNowFinished: {
            Wrapper.accept();
        }
    }
}

