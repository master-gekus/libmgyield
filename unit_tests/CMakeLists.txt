cmake_minimum_required(VERSION 3.5)

project(mgyield_unit_tests)

enable_testing()

include(ExternalProject)

ExternalProject_Add(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  UPDATE_DISCONNECTED 1
  GIT_SHALLOW 1
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=$<CONFIG>
    -Dgtest_force_shared_crt=ON
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/gtest"
    INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(googletest SOURCE_DIR)
set(GTEST_INCLUDE_DIRS
  ${SOURCE_DIR}/googletest/include
  ${SOURCE_DIR}/googlemock/include
  )
include_directories(${GTEST_INCLUDE_DIRS})

ExternalProject_Get_Property(googletest BINARY_DIR)
set(GTEST_LIBS_DIR ${BINARY_DIR}/lib)

add_executable(${PROJECT_NAME}
  mgyield_unit_tests_main.cpp
  mgyield_unit_tests_empty.cpp
  mgyield_unit_tests_int.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${GTEST_INCLUDE_DIRS}
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
)

add_dependencies(${PROJECT_NAME}
  googletest
  )

target_compile_definitions(${PROJECT_NAME}
  PRIVATE -DGTEST_INVOKED
  )

target_include_directories(${PROJECT_NAME} PRIVATE
  ${GTEST_INCLUDE_DIRS}
  )

target_link_libraries(${PROJECT_NAME}
  mgyield
  ${GTEST_LIBS_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}
  ${GTEST_LIBS_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main$<$<CONFIG:Debug>:d>${CMAKE_STATIC_LIBRARY_SUFFIX}
  )

if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME}
    pthread
    )
endif()

add_test(${PROJECT_NAME} ${PROJECT_NAME})
