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
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets Multimedia)

# Define source, header, and UI directories
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/SourceFiles")
set(HEADER_DIR "${CMAKE_SOURCE_DIR}/HeaderFiles")
set(UI_DIR "${CMAKE_SOURCE_DIR}/UIFiles")

# Set AUTOUIC search path to find .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS "${UI_DIR}")

# Automatically add all .cpp, .hpp, and .ui files
file(GLOB_RECURSE PROJECT_SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS "${HEADER_DIR}/*.hpp")
file(GLOB_RECURSE PROJECT_UIS "${UI_DIR}/*.ui")

# Add the resources.qrc file
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
qt_add_resources(RESOURCES "${RESOURCE_DIR}/resources.qrc")

# Add executable target with the appropriate sources
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Client
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UIS}
    )
else()
    add_executable(Client
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_UIS}
    )
endif()

# Include directories for headers
include_directories(${HEADER_DIR})

# Link Qt Widgets library
target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

# Set target properties for macOS and Windows
if(APPLE)
    # macOS settings
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
    # Windows settings
    set_target_properties(Client PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Finalize executable for Qt 6 on both platforms
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Client)
endif()

# Install directives for both platforms
include(GNUInstallDirs)
install(TARGETS Client
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
