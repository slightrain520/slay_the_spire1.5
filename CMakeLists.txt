cmake_minimum_required(VERSION 3.16)
project(SlayTheSpire1.5 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)

set(RESOURCES resources/resources.qrc)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/data
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
