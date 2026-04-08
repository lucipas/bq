cmake_minimum_required(VERSION 3.16)
project(BrowserProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# 1. ADD THIS: Helps your editor (Neovim/VS Code) find headers on Arch
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets WebEngineWidgets)

add_executable(my_browser main.cpp)

target_link_libraries(my_browser PRIVATE 
    Qt6::Widgets 
    Qt6::WebEngineWidgets
)
