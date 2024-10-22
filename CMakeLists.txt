cmake_minimum_required(VERSION 3.25)

find_package(cmake-bare REQUIRED PATHS node_modules/cmake-bare)
find_package(cmake-napi REQUIRED PATHS node_modules/cmake-napi)

project(quickbit_native C)

if(NOT TARGET quickbit)
  add_subdirectory(vendor/libquickbit EXCLUDE_FROM_ALL)
endif()

add_bare_module(quickbit_native_bare)

target_sources(
  ${quickbit_native_bare}
  PRIVATE
    binding.c
  PUBLIC
    $<TARGET_OBJECTS:quickbit>
    $<TARGET_OBJECTS:simdle>
)

target_link_libraries(
  ${quickbit_native_bare}
  PUBLIC
    quickbit
    simdle
)

add_napi_module(quickbit_native_node)

target_sources(
  ${quickbit_native_node}
  PRIVATE
    binding.c
  PUBLIC
    $<TARGET_OBJECTS:quickbit>
    $<TARGET_OBJECTS:simdle>
)

target_link_libraries(
  ${quickbit_native_node}
  PUBLIC
    quickbit
    simdle
)
