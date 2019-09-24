cmake_minimum_required(VERSION 2.6) 

set ( TARGET_NAME Basic )

# Declare project file
project(${TARGET_NAME} C CXX)

# set top level directory
set (TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set ( ENJON_DIR "E:/Development/Enjon/Build/Editor/../../" )
set (ENJON_INC ${ENJON_DIR}/Include)
set (ENJON_BUILD_DIR ${ENJON_DIR}/Build)
set (TARGET_BUILD_DIR "${TARGET_DIR}/Build")
set (ENJON_RELDEBUGINFO_DIR ${ENJON_BUILD_DIR}/RelWithDebInfo)
set (ENJON_RELEASE_DIR ${ENJON_BUILD_DIR}/Release)
set (ENJON_DEBUG_DIR ${ENJON_BUILD_DIR}/Debug)

set (TARGET_RELEASE_DIR "${TARGET_BUILD_DIR}/Release")
set (TARGET_RELEASEDEBUGINFO_DIR "${TARGET_BUILD_DIR}/RelWithDebInfo")
set (TARGET_DEBUG_DIR "${TARGET_BUILD_DIR}/Debug")

# Set build directory for solution
set (EXECUTABLE_OUTPUT_PATH ${TARGET_BUILD_DIR})
set (LIBRARY_OUTPUT_PATH ${TARGET_BUILD_DIR})

# Turn on ability to create folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler flags and build options
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /W1")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD /W1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /W1")

message("cxx FLgs:" ${CMAKE_CXX_FLAGS})

# Add include directories
include_directories(
					${TARGET_DIR}/Source
					${ENJON_DIR}/ThirdParty/Build/Include
					${ENJON_INC}
					)

# Add lib directories
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/Release)
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/Debug)
link_directories(${ENJON_BUILD_DIR}/Debug)
link_directories(${ENJON_BUILD_DIR}/Release)
link_directories(${ENJON_BUILD_DIR}/RelWithDebInfo)

set(TARGET_INC_DIR "${TARGET_DIR}/Source/")
set(TARGET_SOURCE_DIR "${TARGET_DIR}/Source/")
set(TARGET_GENERATED_DIR ${TARGET_BUILD_DIR}/Generator/Linked/)

file( 
		GLOB_RECURSE TARGET_SOURCES

		"${TARGET_INC_DIR}/*.h"
		"${TARGET_INC_DIR}/*.c"
		"${TARGET_SOURCE_DIR}/*.cpp"
	)

set(
		ALL_SRC 
		${TARGET_SOURCES}
		${TARGET_GENERATED_DIR}/${TARGET_NAME}_Generated.cpp
	)

file(
		GLOB_RECURSE REFLECTION_INCLUDES
		"${TARGET_INC_DIR}/*.h"
	)

add_library(${TARGET_NAME} SHARED ${ALL_SRC}) 


# Link libraries to target
target_link_libraries( ${TARGET_NAME} debug Enjond.lib optimized Enjon.lib )
target_link_libraries( ${TARGET_NAME} debug Opengl32.lib optimized Opengl32.lib ) 
target_link_libraries( ${TARGET_NAME} debug SDL2_d.lib optimized SDL2.lib )
target_link_libraries( ${TARGET_NAME} debug SDL2main_d.lib optimized SDL2main.lib )
target_link_libraries( ${TARGET_NAME} debug glew32_d.lib optimized glew32.lib )
target_link_libraries( ${TARGET_NAME} debug glew32s_d.lib optimized glew32s.lib )
target_link_libraries( ${TARGET_NAME} debug freetype_d.lib optimized freetype.lib )
target_link_libraries( ${TARGET_NAME} debug Bullet3Collision_d.lib optimized Bullet3Collision.lib )
target_link_libraries( ${TARGET_NAME} debug Bullet3Dynamics_d.lib optimized Bullet3Dynamics.lib )
target_link_libraries( ${TARGET_NAME} debug Bullet3Common_d.lib optimized Bullet3Common.lib )
target_link_libraries( ${TARGET_NAME} debug LinearMath_d.lib optimized LinearMath.lib )
target_link_libraries( ${TARGET_NAME} debug BulletDynamics_d.lib optimized BulletDynamics.lib )
target_link_libraries( ${TARGET_NAME} debug BulletCollision_d.lib optimized BulletCollision.lib )
target_link_libraries( ${TARGET_NAME} debug fmt_d.lib optimized fmt.lib )
target_link_libraries( ${TARGET_NAME} debug liblz4_d.lib optimized liblz4.lib ) 
target_link_libraries( ${TARGET_NAME} debug assimp-vc140-mt.lib optimized assimp-vc140-mt.lib ) 

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${TARGET_BUILD_DIR})

set_target_properties(
						${TARGET_NAME} 
						PROPERTIES 
						LINKER_LANGUAGE CXX 
					)

set_target_properties(
						${TARGET_NAME} 
						PROPERTIES 
						PDB_NAME "$(TargetName)-$([System.DateTime]::Now.ToString(\"HH_mm_ss_fff\"))"
					)

source_group("Source" FILES ${TARGET_SOURCES})

# Write to .user file for command arguments
if (MSVC)
	file(WRITE ${TARGET_DIR}/Build/${TARGET_NAME}.vcxproj.user
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>
		<Project ToolsVersion=\"14.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
		  <PropertyGroup>
		    <LocalDebuggerCommandArguments>--target-path ${TARGET_DIR}</LocalDebuggerCommandArguments>
		     <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		  </PropertyGroup>
		</Project>" 
	)
endif()

# List of necessary DLLs 
set(
		DLL_LIST

		SDL2.dll
		freetype6.dll
		glew32.dll
		assimp.dll
	)

# List of build directories
set(
		BUILD_DIRECTORIES

		${TARGET_RELEASE_DIR}
		${TARGET_RELEASEDEBUGINFO_DIR}
		${TARGET_DEBUG_DIR}
	)

# Install directories
foreach(dir ${BUILD_DIRECTORIES})
	if (NOT IS_DIRECTORY ${dir})
		file(MAKE_DIRECTORY ${dir})
		execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${dir})
	endif()
endforeach(dir)

# Reflection information
set( TARGET_REFLECTION_DIR ${TARGET_BUILD_DIR}/Generator/ )

# Create directory if doesn't exist
if (NOT IS_DIRECTORY ${TARGET_REFLECTION_DIR})
	file(MAKE_DIRECTORY ${TARGET_REFLECTION_DIR})
	file(MAKE_DIRECTORY ${TARGET_REFLECTION_DIR}/Intermediate/)
	file(MAKE_DIRECTORY ${TARGET_REFLECTION_DIR}/Linked/)
endif()

# Create reflection list for project
file(WRITE ${TARGET_REFLECTION_DIR}/reflection.cfg "#target_dir \"${TARGET_DIR}\"\n") 
file(WRITE ${TARGET_REFLECTION_DIR}/reflection.cfg "#engine_dir \"${ENJON_DIR}\"\n") 
file(APPEND ${TARGET_REFLECTION_DIR}/reflection.cfg "#project_name \"${TARGET_NAME}\"\n") 
file(APPEND ${TARGET_REFLECTION_DIR}/reflection.cfg "#generate_app_bindings true \n") 
file(APPEND ${TARGET_REFLECTION_DIR}/reflection.cfg "#files\n") 

# Add all reflection include files
foreach( file ${REFLECTION_INCLUDES} )

	file(APPEND ${TARGET_REFLECTION_DIR}/reflection.cfg "\"${file}\"\n") 

endforeach(file)

# Remove all previous .pdb files if possible
file(
	GLOB_RECURSE PDB_FILES
	"${TARGET_BUILD_DIR}/Debug/*.pdb"
) 

add_custom_command(
	TARGET ${TARGET_NAME} PRE_BUILD
	COMMAND call ${TARGET_DIR}/Proc/DelPDB.bat ${TARGET_DIR}
) 

# Need to get this working with projects
add_custom_command(
	TARGET ${TARGET_NAME} PRE_BUILD
	COMMAND call ${ENJON_RELEASE_DIR}/Reflection.exe ARGS --target-path ${TARGET_DIR}/
)