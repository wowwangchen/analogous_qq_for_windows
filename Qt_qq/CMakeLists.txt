cmake_minimum_required(VERSION 3.5)

project(Qt_qq VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Multimedia REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        Register.cpp
        moreSlection.cpp
        ChatMainWindow.cpp
        theFriendMessage.cpp
        registerUp.cpp

        Register.h
        moreSlection.h
        ChatMainWindow.h
        theFriendMessage.h
        registerUp.h

        Register.ui
        moreSlection.ui
        ChatMainWindow.ui
        theFriendMessage.ui
        registerUp.ui

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Qt_qq
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Qt_qq APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Qt_qq SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Qt_qq
            ${PROJECT_SOURCES}
            qq.qrc
            moreSlection.h moreSlection.cpp moreSlection.ui
            ChatMainWindow.h ChatMainWindow.cpp ChatMainWindow.ui
            theFriendMessage.h theFriendMessage.cpp theFriendMessage.ui
            registerUp.h registerUp.cpp registerUp.ui
            People.h People.cpp
            ChatLeftWidget.h ChatLeftWidget.cpp ChatLeftWidget.ui
            ChatRightWidget.h ChatRightWidget.cpp ChatRightWidget.ui
        )
    endif()
endif()

target_link_libraries(Qt_qq PRIVATE Qt${QT_VERSION_MAJOR}::Widgets )
target_link_libraries(Qt_qq PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(Qt_qq PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Qt_qq)
endif()
set_target_properties(Qt_qq PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Qt_qq
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Qt_qq)
endif()
