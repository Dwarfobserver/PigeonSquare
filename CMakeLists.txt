
cmake_minimum_required(VERSION 3.8)

set(EXE_NAME PigeonSquare)

project(${EXE_NAME})

set(CMAKE_CXX_STANDARD 14)

# Set OS-specific folders

if(WIN32)
    set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies/windows")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/windows")
elseif(APPLE)
    set(DEPENDENCIES_DIR "${CMAKE_SOURCE_DIR}/dependencies/mac")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/mac")
else()
    message(FATAL_ERROR "Only Windows and Apple OS are supported")
endif()

# Add source files

set(SOURCE_FILES src/main.cpp
        src/Window.cpp src/Window.hpp
        src/Pigeon.cpp src/Pigeon.hpp
        src/PigeonConfig.hpp
        src/World.cpp src/World.hpp
        src/Runnable.cpp src/Runnable.hpp
        src/Commons.hpp)

add_executable(PigeonSquare ${SOURCE_FILES})

# Link SFML

set(CMAKE_MODULE_PATH "${DEPENDENCIES_DIR}/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
if(WIN32)
    set(SFML_ROOT "${DEPENDENCIES_DIR}/SFML")
endif()

find_package(SFML 2.4.2 REQUIRED system window graphics)

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} ${SFML_LIBRARIES})
