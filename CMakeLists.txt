cmake_minimum_required(VERSION 3.16)

project(bq VERSION 0.1 LANGUAGES CXX)

# 1. Standard C++ Requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 2. Find Qt Packages
# QLocalSocket and QLocalServer live in the Network module
find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets WebEngineWidgets)

# 3. Handle Qt's Meta-Object Compiler (MOC) automatically
# This is crucial for signals and slots to work
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 4. Define your executable and source files
# Replace 'main.cpp' and 'ipc_handler.cpp' with your actual file names
add_executable(bq
    ./src/main.cpp
)

# 5. Link the Qt Libraries
target_link_libraries(bq PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Widgets 
    Qt6::WebEngineWidgets
)

# 6. Define a macro for easier debugging (Optional)
# This ensures you can use qDebug() output in the console on Windows
if(WIN32)
    set_target_properties(bq PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()
