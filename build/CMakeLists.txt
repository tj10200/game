cmake_minimum_required(VERSION 2.8)

if ( NOT DEFINED CMAKE_PATHS )
set ( CMAKE_PATHS 1 )

cmake_policy( SET CMP0015 NEW )

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
        add_definitions( -DDEBUG=1 )
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

if ( NOT DEFINED PIC_COMPILED )
    set ( PIC_COMPILED 1 )
    add_definitions ( -fPIC )
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

set ( GLSDK_DIR ${THIRD_PARTY_DIR}/glsdk )

set ( GLSDK_FREEGLUT_DIR ${GLSDK_DIR}/freeglut )
set ( GLSDK_FREEGLUT_INCLUDE_DIR ${GLSDK_FREEGLUT_DIR}/include )
set ( GLSDK_FREEGLUT_LIB_DIR ${GLSDK_FREEGLUT_DIR}/lib )

set ( GLSDK_GLIMG_DIR ${GLSDK_DIR}/glimg )
set ( GLSDK_GLIMG_INCLUDE_DIR ${GLSDK_GLIMG_DIR}/include )
set ( GLSDK_GLIMG_LIB_DIR ${GLSDK_GLIMG_DIR}/lib )

set ( GLSDK_GLLOAD_DIR ${GLSDK_DIR}/glload )
set ( GLSDK_GLLOAD_INCLUDE_DIR ${GLSDK_GLLOAD_DIR}/include )
set ( GLSDK_GLLOAD_LIB_DIR ${GLSDK_GLLOAD_DIR}/lib )

set ( GLSDK_GLLOADGEN_DIR ${GLSDK_DIR}/glloadgen )
set ( GLSDK_GLLOADGEN_INCLUDE_DIR ${GLSDK_GLLOADGEN_DIR}/include )
set ( GLSDK_GLLOADGEN_LIB_DIR ${GLSDK_GLLOADGEN_DIR}/lib )

set ( GLSDK_GLM_DIR ${GLSDK_DIR}/glm )
set ( GLSDK_GLM_INCLUDE_DIR ${GLSDK_GLM_DIR}/include )
set ( GLSDK_GLM_LIB_DIR ${GLSDK_GLM_DIR}/lib )

set ( GLSDK_GLMESH_DIR ${GLSDK_DIR}/glmesh )
set ( GLSDK_GLMESH_INCLUDE_DIR ${GLSDK_GLMESH_DIR}/include )
set ( GLSDK_GLMESH_LIB_DIR ${GLSDK_GLMESH_DIR}/lib )

set ( GLSDK_GLUTIL_DIR ${GLSDK_DIR}/glutil )
set ( GLSDK_GLUTIL_INCLUDE_DIR ${GLSDK_GLUTIL_DIR}/include )
set ( GLSDK_GLUTIL_LIB_DIR ${GLSDK_GLUTIL_DIR}/lib )

set ( BOOST_INCLUDE_DIR ${THIRD_PARTY_DIR}/boost )
set ( BOOST_LIB_DIR ${BOOST_INCLUDE_DIR}/stage/lib )

set ( OPTION_PARSER_DIR ${THIRD_PARTY_DIR}/optionparser )

###########################################################
#       Framework
###########################################################
set ( FRAMEWORK_DIR ${GAME_ROOT_DIR}/framework )
set ( FRAMEWORK_COMMON_DIR ${FRAMEWORK_DIR}/common )
set ( PLUGIN_LOADER_DIR ${FRAMEWORK_DIR}/pluginloader )
set ( EVENT_MANAGER_DIR ${FRAMEWORK_DIR}/eventmanager )

###########################################################
#       Plugins
###########################################################
set ( FRAMEWORK_PLUGINS_DIR ${FRAMEWORK_DIR}/plugins )
set ( GL_MANAGER_DIR ${FRAMEWORK_PLUGINS_DIR}/glmanager )
set ( DISPLAY_PLUGIN_DIR ${FRAMEWORK_PLUGINS_DIR}/display )
set ( KEYBOARD_PLUGIN_DIR ${FRAMEWORK_PLUGINS_DIR}/keyboard )
set ( RENDERABLE_DIR ${FRAMEWORK_PLUGINS_DIR}/renderable )
set ( RENDERABLE_PLUGINS_DIR ${RENDERABLE_DIR}/plugins )

###########################################################
#      COntainers 
###########################################################
set ( CONTAINER_DIR ${GAME_ROOT_DIR}/containers )

endif()
