cmake_minimum_required(VERSION 3.16)

project(QBasic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Test)

# Define the project include directories
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/windows
    ${CMAKE_SOURCE_DIR}/classes
    ${CMAKE_SOURCE_DIR}/classes/expressions
    ${CMAKE_SOURCE_DIR}/classes/statements
)

# Define the Windows resource file
set(WIN64_RESOURCES ${CMAKE_SOURCE_DIR}/resources.rc)

# Define the project sources
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/settings.h
    ${CMAKE_SOURCE_DIR}/main.cpp

    ${CMAKE_SOURCE_DIR}/windows/mainwindow.h
    ${CMAKE_SOURCE_DIR}/windows/mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/windows/mainwindow.ui

    ${CMAKE_SOURCE_DIR}/classes/statement.h
    ${CMAKE_SOURCE_DIR}/classes/statement.cpp
    ${CMAKE_SOURCE_DIR}/classes/parser.h
    ${CMAKE_SOURCE_DIR}/classes/parser.cpp
    ${CMAKE_SOURCE_DIR}/classes/exp.h
    ${CMAKE_SOURCE_DIR}/classes/exp.cpp
    ${CMAKE_SOURCE_DIR}/classes/evalstate.h
    ${CMAKE_SOURCE_DIR}/classes/evalstate.cpp
    ${CMAKE_SOURCE_DIR}/classes/tokenizer.h
    ${CMAKE_SOURCE_DIR}/classes/tokenizer.cpp
    ${CMAKE_SOURCE_DIR}/classes/editor.h
    ${CMAKE_SOURCE_DIR}/classes/editor.cpp
    ${CMAKE_SOURCE_DIR}/classes/utils.h
    ${CMAKE_SOURCE_DIR}/classes/utils.cpp

    ${WIN64_RESOURCES}  # 将Windows资源文件添加到项目源文件中
)

# Define the resource files
qt6_add_resources(PROJECT_SOURCES resources.qrc)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QBasic
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    if(ANDROID)
        add_library(QBasic SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(QBasic
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QBasic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QBasic PRIVATE Qt6::Test)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QBasic)
endif()
set_target_properties(QBasic PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QBasic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QBasic)
endif()