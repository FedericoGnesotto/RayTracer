include("Internal.cmake")
cmake_minimum_required(VERSION 3.16)
project(RayTracer VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

add_executable(RayTracer
    "src/main.cpp"
    "src/Camera.cpp"
    "src/CameraInteraction.cpp"
    "src/Renderer.cpp"
    "src/Scene.cpp"
    "src/Sphere.cpp"
	
    "include/RayTracer/Camera.h"
    "include/RayTracer/CameraInteraction.h"
    "include/RayTracer/Renderer.h"
    "include/RayTracer/Scene.h"
    "include/RayTracer/Sphere.h"

    "include/RayTracer/Math.h"
)

add_library(Eigen INTERFACE IMPORTED)

set_target_properties(Eigen PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "C:/Dev/Git/externals-vs2017-x64/eigen/3.4.0")
# set_target_properties(${TargetName} PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${ARG_COMPILE_DEFINITIONS}")

target_link_libraries(RayTracer PRIVATE Qt5::Widgets Qt5::Core Eigen)

target_include_directories(RayTracer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(EXISTS "Internal.cmake")
	generate_VS_user_file()
endif()