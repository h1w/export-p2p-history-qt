cmake_minimum_required(VERSION 3.5)

project(c2c-trade-history VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(OpenSSL REQUIRED)

set(TS_FILES c2c-trade-history_en_US.ts)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        settingsdialog.h
        settingsdialog.cpp
        settingsdialog.ui
        utils.h
        binance.h
        binance.cpp
        pexpay.h
        pexpay.cpp
        ${TS_FILES}
)

#target_include_directories(c2c-trade-history PRIVATE "C:/Qt/Tools/OpenSSL/Win_x64/include")

add_subdirectory(nlohmann_json)
add_subdirectory(cpr)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(c2c-trade-history
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET c2c-trade-history APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(c2c-trade-history SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(c2c-trade-history
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(c2c-trade-history PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(c2c-trade-history PRIVATE cpr::cpr)
target_link_libraries(c2c-trade-history PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(c2c-trade-history PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(c2c-trade-history PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS c2c-trade-history
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(c2c-trade-history)
endif()
