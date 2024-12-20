cmake_minimum_required(VERSION 3.16)

project(Client VERSION 0.1 LANGUAGES CXX)

# Enable automatic Qt features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Widgets Multimedia Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets Multimedia Xml)

# Define source, header, and UI directories
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/SourceFiles")
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/HeaderFiles")
set(UI_DIR "${CMAKE_SOURCE_DIR}/UIFiles")

# Set AUTOUIC search path to find .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS "${UI_DIR}")

# List all source files
set(PROJECT_SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/ChatClient.cpp
    ${SOURCE_DIR}/ConnectionWindow.cpp
    ${SOURCE_DIR}/EnvLoader.cpp
    ${SOURCE_DIR}/MainWindow.cpp
    ${SOURCE_DIR}/UIManager.cpp
    ${SOURCE_DIR}/WizzEffect.cpp
    ${SOURCE_DIR}/SignupWindow.cpp
)

# List all header files
set(PROJECT_HEADERS
    ${HEADER_DIR}/ChatClient.hpp
    ${HEADER_DIR}/ConnectionWindow.hpp
    ${HEADER_DIR}/EnvLoader.hpp
    ${HEADER_DIR}/MainWindow.hpp
    ${HEADER_DIR}/UIManager.hpp
    ${HEADER_DIR}/WizzEffect.hpp
    ${HEADER_DIR}/SignupWindow.hpp
)

# List UI files
set(PROJECT_UIS
    ${UI_DIR}/ConnectionWindow.ui
    ${UI_DIR}/MainWindow.ui
    ${UI_DIR}/SignupWindow.ui
)

# Add resources
qt_add_resources(PROJECT_RESOURCES resources/resources.qrc)

# Add executable target with the appropriate sources
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UIS}
        ${PROJECT_RESOURCES}
    )
else()
    add_executable(Client
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UIS}
        ${PROJECT_RESOURCES}
    )
endif()

# Include directories for headers
include_directories(${HEADER_DIR})

# Link Qt libraries
target_link_libraries(Client PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Network 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Xml
)

# Set target properties for macOS and Windows
if(APPLE)
    set_target_properties(Client PROPERTIES
        MACOSX_BUNDLE TRUE
    )
    if(${QT_VERSION} VERSION_LESS 6.1.0)
        set_target_properties(Client PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Client
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        )
    endif()
elseif(WIN32)
    set_target_properties(Client PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Finalize executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
endif()

# Install directives
include(GNUInstallDirs)
install(TARGETS Client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)