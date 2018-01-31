#cmake example for eclipse
#cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SDK=1 -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8 ../source
#cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=release -DBUILD_SDK=1 -DARM=1 -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8 ../source
#cmake example for vs2008
#cmake -G "Visual Studio 9 2008" -DCMAKE_BUILD_TYPE="release" -DBUILD_SDK=1 ../source
#cmake -G "Visual Studio 9 2008" ../source

cmake_minimum_required(VERSION 2.8.3)
project(kunhouarm)

IF(CMAKE_BUILD_TYPE MATCHES "release")
message("----------release----------")
ELSE()
message("----------debug------------")
ENDIF()

IF(ARM)
include("CMakeARM_Boost.txt")
ELSE()
include("CMakeX64_Boost.txt")
ENDIF(ARM)

INSTALL(PROGRAMS CMakeLists.txt DESTINATION source/)
INSTALL(PROGRAMS CMakeX64_Boost.txt DESTINATION source/)
INSTALL(PROGRAMS CMakeARM_Boost.txt DESTINATION source/)
INSTALL(PROGRAMS ReadMe.txt DESTINATION source/)

INSTALL(PROGRAMS build_ARM.sh DESTINATION ./)
INSTALL(PROGRAMS build_x64.sh DESTINATION ./)
INSTALL(DIRECTORY ../thridparty DESTINATION ./)

#################
## output path ##
#################

#message(${CMAKE_BINARY_DIR})
#SET_TARGET_PROPERTIES(core PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
IF(ARM)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/../lib/arm)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/../lib/arm)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/../bin/arm)
set(SDK_LIB_PATH "lib/arm")
ELSE()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/../lib/x64)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/../lib/x64)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_BINARY_DIR}/../bin/x64)
set(SDK_LIB_PATH "lib/x64")
ENDIF(ARM)
message(${SDK_LIB_PATH})

###########
## Build ##
###########
set(KUNHOU_ARM_SDK_PATH ${${PROJECT_NAME}_SOURCE_DIR}/sdk/)
message(${KUNHOU_ARM_SDK_PATH})

set(KUNHOU_ARM_THIRD_PARTY_PATH ${${PROJECT_NAME}_SOURCE_DIR}/../thridparty)
message(${KUNHOU_ARM_THIRD_PARTY_PATH})


## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)

include_directories(
  ${Boost_INCLUDE_DIRS}
  
  ${KUNHOU_ARM_SDK_PATH}/include/
  ${KUNHOU_ARM_SDK_PATH}/include/Comm
  ${KUNHOU_ARM_SDK_PATH}/include/robot
  ${KUNHOU_ARM_SDK_PATH}/include/TransferDevice
  ${KUNHOU_ARM_SDK_PATH}/include/xmlrpc
  ${KUNHOU_ARM_SDK_PATH}/include/sensor
  ${KUNHOU_ARM_SDK_PATH}/include/scanmatch
  ${KUNHOU_ARM_SDK_PATH}/include/log4cpp
  ${KUNHOU_ARM_SDK_PATH}/include/log
)

link_directories(${Boost_LIBRARY_DIRS})

ADD_SUBDIRECTORY(src)
IF(BUILD_SDK)
ADD_SUBDIRECTORY(sdk)
ENDIF(BUILD_SDK)
