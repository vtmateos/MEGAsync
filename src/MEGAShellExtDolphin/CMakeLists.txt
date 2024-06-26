project(megasyncplugin)

cmake_minimum_required(VERSION 3.16.0)
include(FeatureSummary)
set(QT_MIN_VERSION "5.0.0")
set(KF5_MIN_VERSION "5.0.0")

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Network)

find_package(ECM 1.2.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS CoreAddons KIO)

set_package_properties(DolphinVcs PROPERTIES
    DESCRIPTION "the Dolphin plugin library"
    URL "http://dolphin.kde.org/"
    TYPE REQUIRED
    PURPOSE "Provides plugin interfaces for Dolphin."
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMMarkNonGuiExecutable)
include(GenerateExportHeader)

set(CMAKE_AUTOMOC ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DWITH_KF5)


#---OVERLAY PLUGIN---
if (KF5_VERSION)
	if (NOT KF5_VERSION VERSION_LESS 5.16.0)
		set(MEGASYNCDOLPHINOVERLAYPLUGIN megasyncdolphinoverlayplugin)
		if (NOT DEFINED PLUGIN_INSTALL_DIR)
				set(PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/qt5/plugins) #so that kcoreaddons install in the right path (see https://github.com/KDE/kcoreaddons/blob/master/KF5CoreAddonsMacros.cmake)
		endif()
		kcoreaddons_add_plugin(${MEGASYNCDOLPHINOVERLAYPLUGIN} INSTALL_NAMESPACE "kf5/overlayicon"
							   JSON megasync-plugin-overlay.json SOURCES megasync-plugin-overlay.cpp)
		target_link_libraries(${MEGASYNCDOLPHINOVERLAYPLUGIN} KF5::CoreAddons KF5::KIOCore KF5::KIOWidgets)

		#---- ICONS ---- 
		set(HICOLOR_INSTALL_DIR ${KDE_INSTALL_FULL_ICONDIR}/hicolor)
		file(GLOB_RECURSE FILES32 "data/emblems/32x32/*")
		file(GLOB_RECURSE FILES64 "data/emblems/64x64/*")
		install( FILES ${FILES32} DESTINATION ${HICOLOR_INSTALL_DIR}/32x32/emblems)
		install( FILES ${FILES64} DESTINATION ${HICOLOR_INSTALL_DIR}/64x64/emblems)		
	endif()
endif()

#---ACTION PLUGIN---
set(MEGASYNCPLUGIN megasyncplugin)
add_library(${MEGASYNCPLUGIN} MODULE megasync-plugin.cpp)
target_link_libraries(${MEGASYNCPLUGIN} KF5::CoreAddons KF5::KIOCore KF5::KIOWidgets)

install(FILES megasync-plugin.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS ${MEGASYNCPLUGIN} DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt5/plugins)
