cmake_minimum_required(VERSION 2.8.8)
project(HVWrapper C CXX)

#To put all the executables in build directory
#SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

enable_language(CXX)

#Added to detect compiler support for C++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################################################################
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} _build_type)

# Setting the C++ compiler to use
set("${CMAKE_CXX_COMPILER_ID}" TRUE CACHE STRING "C++ Compiler")
if (NOT GNU AND NOT Clang AND NOT Intel)
  message(WARNING "Unsupported compiler. Build will likely fail.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(MacroUtilities)
################################################################################

# see if we have CCACHE ( to avoid recompilation on branch switches )
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message(STATUS "found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

#find_package(curses REQUIRED)
#include_directories(${CURSES_INCLUDE_DIR})


find_package(CAENHV REQUIRED)
include_directories(${CAENHV_INCLUDE_DIR})
message(STATUS "CAEN INCLUDE DIR : ${CAENHV_INCLUDE_DIR}")
message(STATUS "CAEN LIBRARIES: ${CAENHV_LIBRARIES}")

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
file(GLOB headers ${headers} ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h )
file(GLOB sources ${sources} ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(STATUS "*************** Trying to create library libHVWrapper **********")
message(STATUS "HEADERS : ${headers}")
message(STATUS "SOURCES : ${sources}")
add_library(HVWrapper SHARED ${headers} ${sources})
target_link_libraries(HVWrapper ${CAENHV_LIBRARIES})

add_library(HVSupply SHARED  
${CMAKE_CURRENT_SOURCE_DIR}/inc/HVData.h  
${CMAKE_CURRENT_SOURCE_DIR}/inc/Loader.h  
${CMAKE_CURRENT_SOURCE_DIR}/inc/HVSupply.h 
${CMAKE_CURRENT_SOURCE_DIR}/src/HVSupply.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/PowerSupplyConfReader.cpp 
)

#target_link_libraries(HVSupply ${ROOT_LIBRARIES} ${CAENHV_LIBRARIES} )

file(GLOB_RECURSE SOURCES2 . $ENV{ROOTSYS}/lib/*.so)
target_link_libraries(HVSupply ${ROOT_LIBRARIES} ${CAENHV_LIBRARIES} )

#message(STATUS "ROOT LIB : ${SOURCES2}")



add_executable(MainWrap  test/MainWrapp.c ${headers} ${sources})
target_link_libraries(MainWrap HVWrapper -lncurses)


add_executable(Read  test/Read.cpp)
target_link_libraries(Read ${CAENHV_LIBRARIES})

add_executable(ReadHVSupply  test/ReadHVSupply.cpp)
target_link_libraries(ReadHVSupply HVSupply)

add_executable(PowerSupplyConfTester  test/PowerSupplyConfTester.cpp)
target_link_libraries(PowerSupplyConfTester HVSupply)

add_executable(ReadPSUsingConfFile test/ReadPSUsingConfFile.cpp)
target_link_libraries(ReadPSUsingConfFile HVSupply)

add_executable(TestWritingVecOfStruct test/TestWritingVecOfStruct.cpp)
target_link_libraries(TestWritingVecOfStruct HVSupply)
