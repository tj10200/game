cmake_minimum_required(VERSION 2.8)

if ( NOT DEFINED CMAKE_PATHS )
set ( CMAKE_PATHS 1 )

MACRO( prepend_path _srcs _path )
    unset( _tmp )
    foreach( src ${${_srcs}} )
        set ( _tmp ${_tmp} ${_path}/${src} )
    endforeach( src ${_srcs} )
    set( ${_srcs} ${_tmp} )
ENDMACRO( prepend_path _srcs )

if ( DEFINED DEBUG AND DEBUG MATCHES 1 )
    if ( NOT DEFINED DEBUG_SET_FLAG )
        set ( DEBUG_SET_FLAG 1 )
        set ( CMAKE_BUILD_TYPE Debug )
    endif()
else()
    if ( NOT DEFINED DEBUG_SET_FLAG )
        set ( DEBUG_SET_FLAG 1 )
        set ( CMAKE_BUILD_TYPE Release )
    endif()
endif()

if ( NOT DEFINED CXX11_COMPILED )
    set ( CXX11_COMPILED 1 )
    add_definitions ( -std=c++0x )
endif()

set ( GAME_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/.. )

###########################################################
#       Third Party Paths
###########################################################
set ( THIRD_PARTY_DIR ${GAME_ROOT_DIR}/thirdparty )
set ( JSONCPP_DIR ${THIRD_PARTY_DIR}/jsoncpp )
set ( JSONCPP_INCLUDE_DIR ${JSONCPP_DIR}/include )
set ( JSONCPP_LIB_DIR ${JSONCPP_DIR}/libs )
set ( GTEST_DIR ${THIRD_PARTY_DIR}/gtest )
set ( GTEST_INCLUDE_DIR ${GTEST_DIR}/include )
set ( GTEST_LIB_DIR ${GTEST_DIR}/lib/.libs )

###########################################################
#       Framework
###########################################################
set ( FRAMEWORK_DIR ${GAME_ROOT_DIR}/framework )
set ( FRAMEWORK_COMMON_DIR ${FRAMEWORK_DIR}/common )
set ( PLUGIN_LOADER_DIR ${FRAMEWORK_DIR}/pluginloader )
set ( GL_MANAGER_DIR ${FRAMEWORK_DIR}/glmanager )
set ( EVENT_MANAGER_DIR ${FRAMEWORK_DIR}/eventmanager )

endif()
