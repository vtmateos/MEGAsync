SET MEGA_QT_DEPLOY_DIR=qt-deploy-x64

IF [%MEGA_QTPATH%]==[] (
	IF NOT [%MEGAQTPATH%]==[] (
		SET MEGA_QTPATH=%MEGAQTPATH%
	) ELSE (
		SET MEGA_QTPATH=C:\Qt\5.12.12\msvc2017_64
	)
)

REM Clean up any previous leftovers
IF EXIST %MEGA_QT_DEPLOY_DIR% (
    rmdir /s /q %MEGA_QT_DEPLOY_DIR%
)

mkdir %MEGA_QT_DEPLOY_DIR%
%MEGA_QTPATH%\bin\windeployqt.exe --no-translations --no-compiler-runtime ^
 --no-opengl-sw --no-system-d3d-compiler --no-angle --no-webkit2 --no-qmltooling ^
 --no-patchqt --no-designercomponents ^
 --dir %MEGA_QT_DEPLOY_DIR% ^
 --qmldir src\MEGASync\gui\onboarding\qml\ ^
 --qmldir src\MEGASync\gui\qml\ ^
 build-x64-windows-mega\Release\MEGAsync.exe ^
 build-x64-windows-mega\Release\MEGAupdater.exe ^
 build-x64-windows-mega\Release\MEGAShellExt.dll

rmdir /s /q %MEGA_QT_DEPLOY_DIR%\bearer
rmdir /s /q %MEGA_QT_DEPLOY_DIR%\scenegraph

IF "%MEGA_SKIP_32_BIT_BUILD%" == "true" (
	GOTO :EOF
)

SET MEGA_QT_DEPLOY_DIR=qt-deploy-x86

REM Clean up any previous leftovers
IF EXIST %MEGA_QT_DEPLOY_DIR% (
    rmdir /s /q %MEGA_QT_DEPLOY_DIR%
)

mkdir %MEGA_QT_DEPLOY_DIR%
%MEGA_QTPATH%\..\msvc2017\bin\windeployqt.exe --no-translations --no-compiler-runtime ^
 --no-opengl-sw --no-system-d3d-compiler --no-angle --no-webkit2 --no-qmltooling ^
 --no-patchqt --no-designercomponents ^
 --dir %MEGA_QT_DEPLOY_DIR% ^
 --qmldir src\MEGASync\gui\onboarding\qml ^
 --qmldir src\MEGASync\gui\qml\ ^
 build-x86-windows-mega\Release\MEGAsync.exe ^
 build-x86-windows-mega\Release\MEGAShellExt.dll
 build-x86-windows-mega\Release\MEGAupdater.exe ^

rmdir /s /q %MEGA_QT_DEPLOY_DIR%\bearer
rmdir /s /q %MEGA_QT_DEPLOY_DIR%\scenegraph
