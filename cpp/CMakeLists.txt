cmake_minimum_required(VERSION 3.10)
project("talibwasm")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# include_directories(src)
include_directories("lib/googletest/include")
add_subdirectory("lib/googletest")

#/**/!(*.c|*.h)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/* ${curdir}/**/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${curdir}/${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(TALIB_SOURCE_DIRECTORIES "${PROJECT_SOURCE_DIR}/lib/ta-lib/src")
# message("TALIB_SOURCE_DIRECTORIES: ${TALIB_SOURCE_DIRECTORIES}")

include_directories(${TALIB_SOURCE_DIRECTORIES} "${PROJECT_SOURCE_DIR}/lib/ta-lib/include")

file(
  GLOB_RECURSE
  TALIB_SOURCES
  FOLLOW_SYMLINKS
  LIST_DIRECTORIES false
  "${PROJECT_SOURCE_DIR}/lib/ta-lib/src/*.c"
  "${PROJECT_SOURCE_DIR}/lib/ta-lib/src/*.h"
)
add_library(ta-lib STATIC ${TALIB_SOURCES})
# message("TALIB_SOURCES: ${TALIB_SOURCES}")

add_subdirectory("src")
add_subdirectory("test")
