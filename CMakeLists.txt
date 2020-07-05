cmake_minimum_required(VERSION 3.5)

project(mgyield LANGUAGES C CXX)

set(MGYIELD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(MGYIELD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC
  ${MGYIELD_INCLUDE_DIR}/mgyield.h
  ${MGYIELD_SOURCE_DIR}/mgyield.cpp
  )

set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${MGYIELD_INCLUDE_DIR}
  )

target_include_directories(${PROJECT_NAME} PRIVATE
  ${MGYIELD_INCLUDE_DIR}
  )

#target_link_libraries(${PROJECT_NAME} PRIVATE
#  ${MGRTP_PORT_LIBRARIES}
#  )

#if (MGRTP_PORT_DEFINITIONS)
#  target_compile_definitions(${PROJECT_NAME} PRIVATE
#    ${MGRTP_PORT_DEFINITIONS}
#    )
#endif()

add_subdirectory(unit_tests)
