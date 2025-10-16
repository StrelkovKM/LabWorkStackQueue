cmake_minimum_required(VERSION 3.5) #Проверка версии CMake.

set(PROJECT_NAME LabWorkStackQueue)
project(${PROJECT_NAME})

set(application SQApplication)

set(sqlib SQLibrary)
set(errorlib ErrorLibrary)
set(stringlib StringLibrary)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLib) 
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ErrorLib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/StringLib)

add_subdirectory(SQApp)
add_subdirectory(SQLib)
add_subdirectory(ErrorLib)
add_subdirectory(StringLib)
add_subdirectory(SQTest)
add_subdirectory(gtest)



