project(MBUtility)
set(PROJECT_NAME "MBUtility")
##BEGIN MBPM_VARIABLES
set(MBPM_DEPENDENCIES 
)
set(MBPM_TARGET_EXTPACKET_LIBRARIES )
set(MBPM_TARGET_COMPILE_OPTIONS )
set(MBPM_TARGET_LINK_OPTIONS )
#MBPM_Functions#variables MBPM_DEPENDENCIES
#MBPM_TARGET_EXTPACKET_LIBRARIES
#MBPM_TARGET_COMPILE_OPTIONS
#MBPM_TARGET_LINK_OPTIONS
#Environment variables MBPM_PACKETS_INSTALL_DIRECTORY
macro(MBPM_UpdateLibraries TargetToUpdate LinkType BuildType)
	set(MBPM_TARGET_EXTPACKET_LIBRARIES "")
	foreach(PACKET IN LISTS MBPM_DEPENDENCIES)
		set(NewLibraryName "")
		set(NewLibraryToAdd "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/${PACKET}/MBPM_Builds/")
		if(${LinkType} STREQUAL "STATIC")
			set(NewLibraryName "${PACKET}_S")
		elseif(${LinkType} STREQUAL "DYNAMIC")
			set(NewLibraryName "${PACKET}_D")
		endif()
		if(${BuildType} STREQUAL "DEBUG")
			set(NewLibraryName "${NewLibraryName}D")
		elseif(${BuildType} STREQUAL "RELEASE")
			set(NewLibraryName "${NewLibraryName}R")
		endif()
		if(UNIX)
			set(NewLibraryName "lib${NewLibraryName}.")
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}a")
			else()
				set(NewLibraryName "${NewLibraryName}so")
			endif()
		else()
			if(${LinkType} STREQUAL "STATIC")
				set(NewLibraryName "${NewLibraryName}.lib")
			else()
				set(NewLibraryName "${NewLibraryName}.dll")				
			endif()
		endif()
		set(NewLibraryToAdd "${NewLibraryToAdd}${NewLibraryName}")
		list(APPEND MBPM_TARGET_EXTPACKET_LIBRARIES ${NewLibraryToAdd})
	endforeach()
endmacro()

macro(MBPM_UpdateTargetVariables TargetToUpdate LinkType BuildType)
	MBPM_UpdateLibraries(${TargetToUpdate} ${LinkType} ${BuildType})
endmacro()

macro(MBPM_UpdateOutput TargetToUpdate)
	set_target_properties(${TargetToUpdate} 
	PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/"
	)
	#set_target_properties(${TargetToUpdate} PROPERTIES PREFIX "")
	#set_target_properties(${TargetToUpdate} PROPERTIES SUFFIX "")
	set_target_properties(${TargetToUpdate} PROPERTIES OUTPUT_NAME ${TargetToUpdate})
endmacro()

macro(MBPM_SetTargetCompileOptions TargetToUpdate LinkType BuildType)
	if(UNIX)

	else()
		if(${BuildType} STREQUAL "DEBUG" AND ${LinkType} STREQUAL "STATIC")
			target_compile_options(${TargetToUpdate} PRIVATE /MTd)
		else()
			target_compile_options(${TargetToUpdate} PRIVATE /MT)
		endif()
	endif()
endmacro()

macro(MBPM_LinkTargetPacketLibraries Target)
	target_link_libraries(${Target} ${MBPM_TARGET_EXTPACKET_LIBRARIES})
endmacro()

macro(MBPM_SetTargetIncludeDirectories Target)
	target_include_directories(${Target} PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/MBUtility/"
	)
endmacro()

macro(MBPM_ApplyTargetConfiguration TargetToUpdate LinkType BuildType)
	MBPM_UpdateTargetVariables(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_UpdateOutput(${TargetToUpdate})
	MBPM_SetTargetCompileOptions(${TargetToUpdate} ${LinkType} ${BuildType})
	MBPM_SetTargetIncludeDirectories(${TargetToUpdate})
endmacro()
##END MBPM_VARIABLES
set(PROJECT_SOURCES 

	"${CMAKE_CURRENT_SOURCE_DIR}//MBCompileDefinitions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBErrorHandling.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBMatrix.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBFiles.cpp"
)
set(PROJECT_HEADERS 
	"${CMAKE_CURRENT_SOURCE_DIR}//MBAlgorithms.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBCompileDefinitions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBErrorHandling.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBInterfaces.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBMathTemplateFunctions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBMatrix.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBRandom.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBStrings.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//TextReader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}//Filesystem.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBFiles.h"
)
set(COMMON_FILES ${PROJECT_SOURCES} ${PROJECT_HEADERS})
set(COMMON_DYNAMIC_LIBRARIES 
)
set(COMMON_STATIC_LIBRARIES 
)
set(COMMON_LIBRARIES ${COMMON_STATIC_LIBRARIES} ${COMMON_DYNAMIC_LIBRARIES})

set(EXECUTABLE_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/MBUtilityTest.cpp"
)


add_library(MBUtility_SD STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBUtility_SD" "STATIC" "DEBUG")
target_compile_features("MBUtility_SD" PRIVATE cxx_std_17)
target_link_libraries("MBUtility_SD" ${COMMON_LIBRARIES})

add_library(MBUtility_SR STATIC ${COMMON_FILES})
MBPM_ApplyTargetConfiguration("MBUtility_SR" "STATIC" "RELEASE")
target_compile_features("MBUtility_SR" PRIVATE cxx_std_17)
target_link_libraries("MBUtility_SR" ${COMMON_LIBRARIES})

add_executable(MBUtility_Test ${COMMON_FILES} ${EXECUTABLE_FILES})
MBPM_ApplyTargetConfiguration("MBUtility_Test" "STATIC" "STATIC")
target_compile_features("MBUtility_Test" PRIVATE cxx_std_17)
target_link_libraries("MBUtility_Test" ${COMMON_LIBRARIES})