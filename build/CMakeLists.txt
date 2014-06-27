cmake_minimum_required(VERSION 2.8)

if ( NOT DEFINED CMAKE_PATHS )
set ( CMAKE_PATHS 1 )

set ( GAME_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} )

###########################################################
#       Third Party Paths
###########################################################
set ( THIRD_PARTY_DIR ${GAME_ROOT_DIR}/thirdparty )
set ( JSONCPP_DIR ${THIRD_PARTY_DIR}/jsoncpp )
set ( JSONCPP_INCLUDE_DIR ${JSONCPP_DIR}/include )
set ( JSONCPP_LIB_DIR ${JSONCPP_DIR}/libs )

endif()
