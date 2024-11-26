cmake_minimum_required(VERSION 3.10)

project(muscle_path)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(VCPKG_DIR "/Users/xiyuchen/vcpkg/installed/x64-osx")
#set(VCPKG_TARGET_TRIPLET "x64-osx" CACHE STRING "")
#set(CMAKE_TOOLCHAIN_FILE "/Users/xiyuchen/vcpkg/scripts/buildsystems/vcpkg.cmake")
#cmake .. -DCMAKE_TOOLCHAIN_FILE=/Users/xiyuchen/vcpkg/scripts/buildsystems/vcpkg.cmake


set(Qt5_DIR /Users/xiyu/Documents/Qt5.14.2/5.14.2/clang_64/lib/cmake/Qt5)
include_directories("/usr/local/include")

find_package(CasADi REQUIRED)
find_package(Qt5 COMPONENTS Widgets OpenGL 3DCore 3DExtras REQUIRED)

message("Project source directory: ${PROJECT_SOURCE_DIR}")

file(GLOB SRC_FILES 
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/math/*.h"
    "${PROJECT_SOURCE_DIR}/src/math/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/parm/*.h"
    "${PROJECT_SOURCE_DIR}/src/parm/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/parm/body/*.h"
    "${PROJECT_SOURCE_DIR}/src/parm/body/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/parm/muscle/*.h"
    "${PROJECT_SOURCE_DIR}/src/parm/muscle/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/parm/joint/*.h"
    "${PROJECT_SOURCE_DIR}/src/parm/joint/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/constraint/*.h"
    "${PROJECT_SOURCE_DIR}/src/constraint/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/objective/*.h"
    "${PROJECT_SOURCE_DIR}/src/objective/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/solveeq/*.h"
    "${PROJECT_SOURCE_DIR}/src/solveeq/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/postprocessing/*.h"
    "${PROJECT_SOURCE_DIR}/src/postprocessing/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/model/*.h"
    "${PROJECT_SOURCE_DIR}/src/model/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/IO/*.h"
    "${PROJECT_SOURCE_DIR}/src/IO/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/json/*.h"
    "${PROJECT_SOURCE_DIR}/src/json/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/json/json/*.h"
    "${PROJECT_SOURCE_DIR}/src/json/json/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/*.h"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/*.ui"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/page/*.h"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/page/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/page/jointtype/*.h"
    "${PROJECT_SOURCE_DIR}/src/qtwindow/page/jointtype/*.cpp")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
#add_library(${CMAKE_PROJECT_NAME} OBJECT ${SRC_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CasADi_INCLUDE_DIRS})


target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE casadi)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::OpenGL Qt5::3DCore Qt5::3DExtras)

