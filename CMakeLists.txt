project(MBUtility)
set(PROJECT_NAME "MBUtility")
##BEGIN MBPM_VARIABLES
set(MBPM_DEPENDENCIES 
)
set(MBPM_SystemLibraries
Ws2_32.lib
Secur32.lib
Bcrypt.lib
Mfplat.lib
opengl32
Mfuuid.lib
	Strmiids.lib
)
set(MBPM_DependanciesIncludeDirectories
)
set(MBPM_TARGET_EXTPACKET_LIBRARIES )
set(MBPM_TARGET_COMPILE_OPTIONS )
set(MBPM_TARGET_LINK_OPTIONS )
#MBPM_Functions
set(MB_BUILD_TYPE "Debug") #Abstraction around CMAKE_BUILD_TYPE potentially being empty
set(MBPM_EXTPACKETS_BUILD_DIRECTORIES )
set(MBPM_EXTPACKETS_LIBRARIES)
macro(MBPM_UpdateLibraries)
	set(MBPM_EXTPACKETS_LIBRARIES "")
	set(MBPM_EXTPACKETS_BUILD_DIRECTORIES "")
	foreach(PACKET IN LISTS MBPM_DEPENDENCIES)
		set(NewLibraryName "")
		set(PacketName ${PACKET})
		set(LibraryName ${PACKET})
		if(${PACKET} MATCHES "#")
			string(REPLACE "#" ";" PACKET_PART_LIST ${PACKET})
			list(GET PACKET_PART_LIST 0 PacketName)
			list(GET PACKET_PART_LIST 1 LibraryName)
		endif()
		set(NewLibraryDirectory "$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/${PacketName}/MBPM_Builds/${MB_BUILD_TYPE}/")
		list(APPEND MBPM_EXTPACKETS_LIBRARIES ${LibraryName})
		list(APPEND MBPM_EXTPACKETS_BUILD_DIRECTORIES ${NewLibraryDirectory})
	endforeach()
endmacro()
set(MBPM_VariablesUpdated "FALSE")
macro(MBPM_UpdateVariables)
	if(${MBPM_VariablesUpdated} STREQUAL "FALSE")
		if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "" AND DEFINED CMAKE_BUILD_TYPE)
			set(MB_BUILD_TYPE ${CMAKE_BUILD_TYPE})
		endif()
		MBPM_UpdateLibraries()
	endif()	
	set(MBPM_VariablesUpdated "TRUE")
endmacro()

macro(MBPM_UpdateOutput TargetToUpdate)
	set(BuildDirectorySuffix ${MB_BUILD_TYPE})
	set_target_properties(${TargetToUpdate} 
	PROPERTIES 
	ARCHIVE_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/${BuildDirectorySuffix}/>
	LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/${BuildDirectorySuffix}/>
	RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_CURRENT_SOURCE_DIR}/MBPM_Builds/${BuildDirectorySuffix}/>
	)
endmacro()

macro(MBPM_LinkTargetPacketLibraries Target)
	target_link_directories(${Target} PRIVATE ${MBPM_EXTPACKETS_BUILD_DIRECTORIES})
	target_link_libraries(${Target} ${MBPM_EXTPACKETS_LIBRARIES})
	target_link_libraries(${Target} ${MBPM_SystemLibraries})
endmacro()

macro(MBPM_SetTargetIncludeDirectories Target)
	target_include_directories(${Target} PUBLIC 
	"${CMAKE_CURRENT_SOURCE_DIR}/"
	"$ENV{MBPM_PACKETS_INSTALL_DIRECTORY}/"
	)
	target_include_directories(${Target} PUBLIC ${MBPM_DependanciesIncludeDirectories})
endmacro()

macro(MBPM_ApplyConfiguration TargetToUpdate)
	MBPM_UpdateVariables()
	MBPM_UpdateOutput(${TargetToUpdate})
	MBPM_SetTargetIncludeDirectories(${TargetToUpdate})
	MBPM_LinkTargetPacketLibraries(${TargetToUpdate})
endmacro()
##END MBPM_VARIABLES
set(PROJECT_SOURCES 

	"${CMAKE_CURRENT_SOURCE_DIR}//MBCompileDefinitions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBErrorHandling.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}//MBMatrix.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/MBFiles.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/MBInterfaces.cpp"
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
set(COMMON_STATIC_LIBRARIES ${MBPM_SystemLibraries}
)
set(COMMON_LIBRARIES ${COMMON_STATIC_LIBRARIES} ${COMMON_DYNAMIC_LIBRARIES})

set(EXECUTABLE_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/MBUtilityTest.cpp"
)


add_library(MBUtility STATIC ${COMMON_FILES})
MBPM_ApplyConfiguration("MBUtility" "STATIC" "DEBUG")
target_compile_features("MBUtility" PRIVATE cxx_std_17)

add_executable(MBUtility_Test ${COMMON_FILES} ${EXECUTABLE_FILES})
MBPM_ApplyConfiguration("MBUtility_Test")
target_compile_features("MBUtility_Test" PRIVATE cxx_std_17)
target_link_libraries("MBUtility_Test" ${COMMON_LIBRARIES})
