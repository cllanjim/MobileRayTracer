cmake_minimum_required( VERSION 3.6.0 )

execute_process( COMMAND git clone https://github.com/g-truc/glm
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND git clone https://github.com/google/googletest
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND git clone https://github.com/syoyo/tinyobjloader
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

execute_process( COMMAND git clone https://github.com/Microsoft/GSL
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( glm )
add_subdirectory( googletest )
add_subdirectory( tinyobjloader )

if( NOT DEFINED ANDROID_ABI )
  add_subdirectory( GSL )
endif()
