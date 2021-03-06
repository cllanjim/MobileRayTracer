cmake_minimum_required( VERSION 3.6.0 )

project( GoogleTest VERSION 1.0.0.0 LANGUAGES CXX )

file( GLOB_RECURSE GTEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp" )

add_executable( ${PROJECT_NAME} ${GTEST_SOURCES} )
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}" )

target_include_directories( ${PROJECT_NAME} PUBLIC "${THIRD_PARTY_HEADERS}/googletest/googletest/include" )
target_include_directories( ${PROJECT_NAME} PUBLIC "/usr/include/libcxxabi" )


target_compile_options( ${PROJECT_NAME} PRIVATE ${COMMON_FLAGS} )
target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:${COMMON_FLAGS_DEBUG}> )
target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:RELEASE>:${COMMON_FLAGS_RELEASE}> )
target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-global-constructors -Wno-weak-vtables )

include( CheckCXXCompilerFlag )
CHECK_CXX_COMPILER_FLAG( "-Wno-zero-as-null-pointer-constant" FLAG__WNO_ZERO_AS_NULL__SUPPORTED )

if( FLAG__WNO_ZERO_AS_NULL__SUPPORTED )
  target_compile_options( ${PROJECT_NAME} PRIVATE -Wno-zero-as-null-pointer-constant )
endif()

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

if( NOT DEFINED ANDROID_ABI )
  target_compile_options( ${PROJECT_NAME} PRIVATE -flto=full )
endif()

target_link_libraries( ${PROJECT_NAME}
PRIVATE Components gtest
general "${COMMON_LINKER_FLAGS}"
debug "${COMMON_LINKER_FLAGS_DEBUG}"
optimized "${COMMON_LINKER_FLAGS_RELEASE}"
)
