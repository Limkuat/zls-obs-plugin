cmake_minimum_required(VERSION 3.21)
project(obs-zls-plugin C)

set(CMAKE_C_STANDARD 17)

include(external/FindLibOBS.cmake)
include(external/OBSPluginsHelper.cmake)

find_package(LibObs REQUIRED)

set(zls-plugin_SOURCES
        src/zls-plugin.c
        src/zls-source.c)

add_library(zls-plugin MODULE
        ${zls-plugin_SOURCES})
target_link_libraries(zls-plugin
        libobs)

install_external_plugin_with_data(zls-plugin data)
