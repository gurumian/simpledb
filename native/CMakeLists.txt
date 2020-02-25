cmake_minimum_required(VERSION 3.0)


if (POLICY CMP0042)
  cmake_policy (SET CMP0042 NEW)
endif (POLICY CMP0042)

if (POLICY CMP0063)
  cmake_policy (SET CMP0063 NEW)
endif (POLICY CMP0063)

project (simpledb)

# CMakeLists for libsimpledb
# Run with -DTEST=ON to build unit tests
#option(TEST "Built unit tests" OFF)
option(BUILD_EXAMPLE "Build examples" ON)

FIND_PACKAGE (Threads)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})


set (CMAKE_CXX_STANDARD 11)


set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/module
    ${CMAKE_MODULE_PATH})

set(simpledb_base_dir ${CMAKE_CURRENT_LIST_DIR})
set(simpledb_src_dir ${simpledb_base_dir}/src/)
set(simpledb_inc_dir ${simpledb_base_dir}/include)
set(simpledb_lib_name "simpledb_native")

set(simpledb_source_files
  ${simpledb_src_dir}/connection.cc
  ${simpledb_src_dir}/result_set.cc
  ${simpledb_src_dir}/result_set.cc
  ${simpledb_src_dir}/statement.cc
  ${simpledb_src_dir}/prepared_statement.cc
)

set(simpledb_header_files
  ${simpledb_inc_dir}/simpledb/simpledb.h
)

find_package(SQLite3 REQUIRED)
include_directories(${SQLITE3_INCLUDE_DIR})
set(LIBS ${LIBS} ${SQLITE3_LIBRARY})

find_package(Glog)
include_directories(${GLOG_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLOG_LIBRARIES})

if (GLOG_FOUND)
add_definitions(-DUSE_GLOG)
endif (GLOG_FOUND)

set(LIBS ${LIBS} ${CMAKE_DL_LIBS})

include_directories(${simpledb_inc_dir})
include_directories(${simpledb_inc_dir}/simpledb)
include_directories(${simpledb_src_dir})

#add_library(${simpledb_lib_name} STATIC SHARED ${simpledb_source_files})
add_library(${simpledb_lib_name} SHARED ${simpledb_source_files})

configure_file ("${simpledb_inc_dir}/simpledb/config.h.in"
                "${simpledb_inc_dir}/simpledb/config.h" )

if (BUILD_EXAMPLE)
add_executable(example ${simpledb_base_dir}/example/example.cc)
target_link_libraries(example ${simpledb_lib_name} ${LIBS})

add_executable(example_create_table ${simpledb_base_dir}/example/example_create_table.cc)
target_link_libraries(example_create_table ${simpledb_lib_name} ${LIBS})

add_executable(example_insert ${simpledb_base_dir}/example/example_insert.cc)
target_link_libraries(example_insert ${simpledb_lib_name} ${LIBS})

add_executable(example_select ${simpledb_base_dir}/example/example_select.cc)
target_link_libraries(example_select ${simpledb_lib_name} ${LIBS})

add_executable(example_update ${simpledb_base_dir}/example/example_update.cc)
target_link_libraries(example_update ${simpledb_lib_name} ${LIBS})

add_executable(example_delete ${simpledb_base_dir}/example/example_delete.cc)
target_link_libraries(example_delete ${simpledb_lib_name} ${LIBS})
endif (BUILD_EXAMPLE)


install(TARGETS ${simpledb_lib_name} DESTINATION lib)
install(FILES ${simpledb_header_files} DESTINATION include/simpledb)
#install(FILES ${simpledb_inc_dir}/simpledb DESTINATION include/simpledb)