cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0026 OLD)
project(RayTracer VERSION 0.0.0 LANGUAGES CXX)

# generate user file so the Visual Studio finds the executables automatically
function(generate_VS_user_file)
    set(QtName Qt5)
	cmake_parse_arguments(ARG "NO_EXPORT" "TARGET" "PATH_DEBUG;PATH_RELEASE" ${ARGN})
	if("${ARG_TARGET}" STREQUAL "")
		set(TargetName ${PROJECT_NAME})
	else()
		set(TargetName ${ARG_TARGET})
	endif()

	if(WIN32)
		get_target_property(TargetType ${TargetName} TYPE)
		get_target_property(ExeDebug RayTracer LOCATION_DEBUG)
		get_target_property(ExeRelease RayTracer LOCATION_RELEASE)
		get_filename_component(ExeLocationDebug "${ExeDebug}" DIRECTORY)
		get_filename_component(ExeLocationRelease "${ExeRelease}" DIRECTORY)


        set(ExtraPathDebug "${${QtName}_DIR}/../../../bin")
        set(ExtraPathRelease "${${QtName}_DIR}/../../../bin")
		if(${TargetType} STREQUAL "EXECUTABLE")
			set(USERFILE_COMMAND_DEBUG "$(TargetPath)")
			set(USERFILE_COMMAND_RELEASE "$(TargetPath)")
			set(USERFILE_WORKING_DIRECTORY_DEBUG "$(ProjectDir)")
			set(USERFILE_WORKING_DIRECTORY_RELEASE "$(ProjectDir)")
			set(USERFILE_ENVIRONMENT_DEBUG "PATH=${ExtraPathDebug};$(PATH)\nQT_PLUGIN_PATH=${${QtName}_DIR}/../../../plugins")
			set(USERFILE_ENVIRONMENT_RELEASE "PATH=${ExtraPathRelease};$(PATH)\nQT_PLUGIN_PATH=${${QtName}_DIR}/../../../plugins")
		endif()

		configure_file("${CMAKE_CURRENT_LIST_DIR}/UserTemplate.user.in" ${CMAKE_CURRENT_BINARY_DIR}/${TargetName}.vcxproj.user @ONLY)
		set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TargetName})
		message(STATUS "Created VS user file ${CMAKE_CURRENT_BINARY_DIR}/${TargetName}.vcxproj.user")
	endif()
endfunction()

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

generate_VS_user_file()