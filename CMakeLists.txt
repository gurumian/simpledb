cmake_minimum_required(VERSION 3.0)


if (POLICY CMP0042)
  cmake_policy (SET CMP0042 NEW)
endif (POLICY CMP0042)

if (POLICY CMP0063)
  cmake_policy (SET CMP0063 NEW)
endif (POLICY CMP0063)

project (simpledbc)

FIND_PACKAGE (Threads)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckCXXSourceCompiles.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CheckSymbolExists.cmake)

set (CMAKE_CXX_STANDARD 11)

add_definitions(-D__STDC_CONSTANT_MACROS)


set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/native/module
    ${CMAKE_MODULE_PATH})

set(BUILD_EXAMPLE OFF CACHE BOOL "Build example")
add_subdirectory(native)

find_package(SQLite3)
include_directories(${SQLITE3_INCLUDE_DIRS})
set(LIBS ${LIBS} ${SQLITE3_LIBRARIES})

set(simpledb_base_dir ${CMAKE_CURRENT_LIST_DIR})
set(simpledb_src_dir ${simpledb_base_dir}/src)
set(simpledb_inc_dir ${simpledb_base_dir}/native/include)

find_package(Glog)

if (GLOG_FOUND)
add_definitions(-DUSE_GLOG)
include_directories(${GLOG_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLOG_LIBRARIES})
endif (GLOG_FOUND)

set(simpledb_source_files
  ${simpledb_src_dir}/addon.cc
  ${simpledb_src_dir}/connection_wrap.cc
  ${simpledb_src_dir}/statement_wrap.cc
  ${simpledb_src_dir}/prepared_statement_wrap.cc
  ${simpledb_src_dir}/result_set_wrap.cc
)

execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
#target_include_directories(simpledb PRIVATE ${NODE_ADDON_API_DIR})
include_directories(${NODE_ADDON_API_DIR})

include_directories(${simpledb_inc_dir})
include_directories(${simpledb_base_dir}/native/include/simpledb)
include_directories(${simpledb_base_dir}/native/src)

include_directories(${CMAKE_JS_INC})
add_library(simpledbc SHARED ${simpledb_source_files} ${CMAKE_JS_SRC})
set_target_properties(simpledbc PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(simpledbc ${CMAKE_JS_LIB} simpledb_native ${LIBS})