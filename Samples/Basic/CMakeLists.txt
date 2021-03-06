cmake_minimum_required(VERSION 2.6) 

set ( PROJ_NAME Basic )

# Declare project file
project( ${PROJ_NAME} C CXX)

# set top level directory
set (PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set (ENJON_DIR "C:/Users/mrfre/Documents/Development/Enjon/Build/Editor/../../")
set (ENJON_INC ${ENJON_DIR}/Include)
set (ENJON_BUILD_DIR ${ENJON_DIR}/Build)
set (PROJ_BUILD_DIR ${PROJ_DIR}/Build)
set (ENJON_RELDEBUGINFO_DIR ${ENJON_BUILD_DIR}/RelWithDebInfo)
set (ENJON_RELEASE_DIR ${ENJON_BUILD_DIR}/Release)
set (ENJON_DEBUG_DIR ${ENJON_BUILD_DIR}/Debug)

set (PROJ_RELEASE_DIR "${PROJ_BUILD_DIR}/Release")
set (PROJ_RELEASEDEBUGINFO_DIR "${PROJ_BUILD_DIR}/RelWithDebInfo")
set (PROJ_DEBUG_DIR "${PROJ_BUILD_DIR}/Debug")

# Set build directory for solution
set (EXECUTABLE_OUTPUT_PATH ${PROJ_BUILD_DIR})
set (LIBRARY_OUTPUT_PATH ${PROJ_BUILD_DIR})

# Turn on ability to create folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler flags and build options
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /W1")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD /W1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /W1")
set(CMAKE_CXX_STANDARD 11)

set(
		ENJON_INCLUDES 
		${ENJON_DIR}/ThirdParty/Build/Include 
		${ENJON_DIR}/ThirdParty/Build/Include/Bullet 
		${ENJON_INC}
)

message("cxx FLgs:" ${CMAKE_CXX_FLAGS})

# Add include directories
include_directories(
	${PROJ_DIR}/Source
${ENJON_INCLUDES}

)

set(
	ENJON_LIB_DIRS

	${ENJON_DIR}/ThirdParty/Build/Libraries/Windows/Release
	${ENJON_DIR}/ThirdParty/Build/Libraries/Windows/Debug
	${ENJON_DIR}/ThirdParty/Build/Libraries/Release
	${ENJON_DIR}/ThirdParty/Build/Libraries/Debug
	${ENJON_DIR}/ThirdParty/Build/Libraries/OSX/Release
	${ENJON_DIR}/ThirdParty/Build/Libraries/OSX/Debug
	${ENJON_BUILD_DIR}/Debug
	${ENJON_BUILD_DIR}/Release
	${ENJON_BUILD_DIR}/RelWithDebInfo
)

# Add lib directories
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/Windows/Release)
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/Windows/Debug)
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/Release)
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/Debug)
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/OSX/Release)
link_directories(${ENJON_DIR}/ThirdParty/Build/Libraries/OSX/Debug)
link_directories(${ENJON_BUILD_DIR}/Debug)
link_directories(${ENJON_BUILD_DIR}/Release)
link_directories(${ENJON_BUILD_DIR}/RelWithDebInfo)

#set(PROJ_INC_DIR "#PROJ_INC_DIR")

set(PROJ_INC_DIR ${PROJ_DIR}/Source/)
set(PROJ_SOURCE_DIR ${PROJ_DIR}/Source/)
set(PROJ_GENERATED_DIR ${PROJ_BUILD_DIR}/Generator/Linked/)

if (NOT IS_DIRECTORY ${PROJ_GENERATED_DIR})
	file(MAKE_DIRECTORY ${PROJ_GENERATED_DIR})
endif()

if (NOT EXISTS ${PROJ_GENERATED_DIR}/${PROJ_NAME}_Generated.cpp)
	file(WRITE ${PROJ_GENERATED_DIR}/${PROJ_NAME}_Generated.cpp "// ${PROJ_NAME}_Generated.cpp\n")
endif()

file( 
		GLOB_RECURSE PROJ_SOURCES

		"${PROJ_INC_DIR}/*.h"
		"${PROJ_SOURCE_DIR}/*.c"
		"${PROJ_SOURCE_DIR}/*.cpp"
	)

set(
		ALL_SRC 
		${PROJ_SOURCES}
		${PROJ_GENERATED_DIR}/${PROJ_NAME}_Generated.cpp
	)

file(
		GLOB_RECURSE REFLECTION_INCLUDES
		"${PROJ_INC_DIR}/*.h"
	)

if (NOT MSVC)
	find_package(SDL2 REQUIRED)
	include_directories(${SDL_INCLUDE_DIRS})
endif()

add_library(${PROJ_NAME} SHARED ${ALL_SRC})

# Link libraries to target
if (MSVC)
	target_link_libraries( ${PROJ_NAME} debug Enjond.lib optimized Enjon.lib )
	target_link_libraries( ${PROJ_NAME} debug Opengl32.lib optimized Opengl32.lib ) 
	target_link_libraries( ${PROJ_NAME} debug SDL2_d.lib optimized SDL2.lib )
	target_link_libraries( ${PROJ_NAME} debug SDL2main_d.lib optimized SDL2main.lib )
	target_link_libraries( ${PROJ_NAME} debug glew32_d.lib optimized glew32.lib )
	target_link_libraries( ${PROJ_NAME} debug glew32s_d.lib optimized glew32s.lib )
	target_link_libraries( ${PROJ_NAME} debug freetype_d.lib optimized freetype.lib )
	target_link_libraries( ${PROJ_NAME} debug assimp.lib optimized assimp_d.lib ) 
else()
	target_link_libraries( ${PROJ_NAME} Enjon.a )
	target_link_libraries( ${PROJ_NAME} assimp.a )
	target_link_libraries( ${PROJ_NAME} IrrXML.a )
	target_link_libraries( ${PROJ_NAME} SDL2main.a )
	target_link_libraries( ${PROJ_NAME} fmt.a )
	target_link_libraries( ${PROJ_NAME} GLEW )
	target_link_libraries( ${PROJ_NAME} nfd )
	target_link_libraries( ${PROJ_NAME} freetype.a )
	target_link_libraries( ${PROJ_NAME} m )
	target_link_libraries( ${PROJ_NAME} z )
	target_link_libraries( ${PROJ_NAME} iconv )
	target_link_libraries( ${PROJ_NAME} png )
	target_link_libraries( ${PROJ_NAME} bz2 )
	target_link_libraries( ${PROJ_NAME} "-framework CoreAudio" )
	target_link_libraries( ${PROJ_NAME} "-framework CoreMedia" )
	target_link_libraries( ${PROJ_NAME} "-framework Cocoa" )
	target_link_libraries( ${PROJ_NAME} "-framework CoreServices" )
	target_link_libraries( ${PROJ_NAME} "-framework CoreAudioKit" )
	target_link_libraries( ${PROJ_NAME} "-framework OpenGL" )
	target_link_libraries( ${PROJ_NAME} "-framework CoreFoundation" )
	target_link_libraries( ${PROJ_NAME} "-framework ForceFeedback" )
	target_link_libraries( ${PROJ_NAME} "-framework Carbon" )
	target_link_libraries( ${PROJ_NAME} "-framework IOKit" )
	target_link_libraries( ${PROJ_NAME} "-framework AudioToolbox" )
	target_link_libraries( ${PROJ_NAME} "-framework AudioUnit" )
	target_link_libraries( ${PROJ_NAME} "-framework CoreVideo" )
	target_link_libraries( ${PROJ_NAME} "-framework Metal" )
	target_link_libraries( ${PROJ_NAME} ${SDL2_LIBRARIES} )
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJ_BUILD_DIR})

set_target_properties(
						${PROJ_NAME} 
						PROPERTIES 
						LINKER_LANGUAGE CXX 
					)

set_target_properties(
						${PROJ_NAME} 
						PROPERTIES 
						PDB_NAME "$(TargetName)-$([System.DateTime]::Now.ToString(\"HH_mm_ss_fff\"))"
					)

source_group("Source" FILES ${PROJ_SOURCES})

# Write to .user file for command arguments
if (MSVC)
	file(WRITE ${PROJ_BUILD_DIR}/${PROJ_NAME}.vcxproj.user
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>
		<Project ToolsVersion=\"14.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
		  <PropertyGroup>
		    <LocalDebuggerCommandArguments>--target-path ${PROJ_DIR}</LocalDebuggerCommandArguments>
		     <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
		  </PropertyGroup>
		</Project>" 
	)
endif()

# List of necessary DLLs 
set(
		DLL_LIST

		SDL2.dll
		freetype.dll
		glew32.dll
		assimp-vc142-mt.dll
		assimp-vc142-mtd.dll
	)

# List of build directories
set(
		BUILD_DIRECTORIES

		${PROJ_RELEASE_DIR}
		${PROJ_RELEASEDEBUGINFO_DIR}
		${PROJ_DEBUG_DIR}
	)

# Install directories
foreach(dir ${BUILD_DIRECTORIES})
	if (NOT IS_DIRECTORY ${dir})
		file(MAKE_DIRECTORY ${dir})
		execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${dir})
	endif()
endforeach(dir)

# Reflection information
set( PROJ_REFLECTION_DIR ${PROJ_BUILD_DIR}/Generator/ )

# Create directory if doesn't exist
if (NOT IS_DIRECTORY ${PROJ_REFLECTION_DIR})
	file(MAKE_DIRECTORY ${PROJ_REFLECTION_DIR})
	file(MAKE_DIRECTORY ${PROJ_REFLECTION_DIR}/Intermediate/)
	file(MAKE_DIRECTORY ${PROJ_REFLECTION_DIR}/Linked/)
endif()

if (NOT IS_DIRECTORY ${PROJ_REFLECTION_DIR}/Intermediate/)
	file(MAKE_DIRECTORY ${PROJ_REFLECTION_DIR}/Intermediate/)
endif()

if (NOT IS_DIRECTORY ${PROJ_REFLECTION_DIR}/Linked/)
	file(MAKE_DIRECTORY ${PROJ_REFLECTION_DIR}/Linked/)
endif()

# Create reflection list for project
file(WRITE ${PROJ_REFLECTION_DIR}/reflection.cfg "#target_dir \"${PROJ_DIR}\"\n") 
file(WRITE ${PROJ_REFLECTION_DIR}/reflection.cfg "#engine_dir \"${ENJON_DIR}\"\n") 
file(APPEND ${PROJ_REFLECTION_DIR}/reflection.cfg "#project_name \"${PROJ_NAME}\"\n") 
file(APPEND ${PROJ_REFLECTION_DIR}/reflection.cfg "#generate_app_bindings true \n") 
file(APPEND ${PROJ_REFLECTION_DIR}/reflection.cfg "#files\n") 

# Add all reflection include files
foreach( file ${REFLECTION_INCLUDES} )
	file(APPEND ${PROJ_REFLECTION_DIR}/reflection.cfg "\"${file}\"\n") 
endforeach(file)

# Remove all previous .pdb files if possible
if (MSVC)
	file(
		GLOB_RECURSE PDB_FILES
		"${PROJ_BUILD_DIR}/Debug/*.pdb"
	) 

	add_custom_command(
		TARGET ${PROJ_NAME} PRE_BUILD
		COMMAND call ${PROJ_DIR}/Proc/DelPDB.bat ${PROJ_DIR}
	) 

	# Need to get this working with projects
	add_custom_command(
		TARGET ${PROJ_NAME} PRE_BUILD
		COMMAND call ${ENJON_RELEASE_DIR}/Reflection.exe ARGS --target-path ${PROJ_DIR}/
	)
endif()
