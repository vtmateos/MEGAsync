/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 2.12
import QtQuick.Controls 2.12
//import Onboarding 1.0

LoginPageForm {

    createAccountButton.onClicked: {
        registerStack.replace(registerPage)
    }
    loginButton.onClicked: {
        if(email.length !== 0 && password.length !== 0)
        {
            OnboardCpp.onLoginClicked({/* [OnboardEnum.EMAIL]:*/ email, /*[OnboardEnum.PASSWORD]:*/ password })
        }
    }
    Component{
        id: registerPage
        RegisterPage{}
    }
}



