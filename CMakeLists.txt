
cmake_minimum_required(VERSION 3.8)

set(EXE_NAME PigeonSquare)

project(${EXE_NAME})

set(CMAKE_CXX_STANDARD 17)

# Set OS-specific dependencies

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

set(SOURCE_FILES
        main.cpp)

add_executable(PigeonSquare ${SOURCE_FILES})

# Link SFML

set(SFML_ROOT "${DEPENDENCIES_DIR}/SFML")

set(CMAKE_MODULE_PATH "${DEPENDENCIES_DIR}/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 REQUIRED system window graphics)

include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXE_NAME} ${SFML_LIBRARIES})
