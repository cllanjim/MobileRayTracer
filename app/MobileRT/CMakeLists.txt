cmake_minimum_required( VERSION 3.6.0 )

project( MobileRT VERSION 1.0.0.0 LANGUAGES CXX )

file( GLOB_RECURSE MOBILE_RT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp" )

add_library( ${PROJECT_NAME} SHARED ${MOBILE_RT_SOURCES} )
set_target_properties( ${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${MOBILE_RT_HEADERS}" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${GLM_HEADERS}" )

target_compile_options( ${PROJECT_NAME} PRIVATE ${COMMON_FLAGS} )
target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:${COMMON_FLAGS_DEBUG}> )
target_compile_options( ${PROJECT_NAME} PRIVATE $<$<CONFIG:RELEASE>:${COMMON_FLAGS_RELEASE}> )

target_compile_options( ${PROJECT_NAME} PRIVATE $<$<NOT:$<BOOL:ANDROID_ABI MATCHES "^mips(64)?$" >>:-flto=full> )

target_link_libraries( ${PROJECT_NAME} PRIVATE glm )

if( DEFINED ANDROID_ABI )
    target_link_libraries( ${PROJECT_NAME} PUBLIC log )
else()
    target_compile_options( ${PROJECT_NAME} PRIVATE -flto=full )
    target_link_libraries( ${PROJECT_NAME} PUBLIC c++ pthread )
endif()

target_link_libraries( ${PROJECT_NAME}
PUBLIC atomic PRIVATE
general "${COMMON_LINKER_FLAGS}"
debug "${COMMON_LINKER_FLAGS_DEBUG}"
optimized "${COMMON_LINKER_FLAGS_RELEASE}"
)
