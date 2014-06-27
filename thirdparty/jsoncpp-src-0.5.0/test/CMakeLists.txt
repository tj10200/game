cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED JSONCPP_TEST )
set ( JSONCPP_TEST 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )

##
#   Includes
##
include_directories ( ${JSONCPP_INCLUDE_DIR}
                      ${GTEST_INCLUDE_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( ${JSONCPP_LIB_DIR}
                   ${GTEST_LIB_DIR}
                 )

##
#   Source Files
##
set ( JSONCPP_TEST_SOURCES simpleJsonTest.cpp )

##
#   Libs
##
set ( JSONCPP_TEST_LIBS json_linux-gcc-4.6_libmt;
                        gtest;
                        gtest_main;
                        pthread  )

##
# Targets
##
add_executable ( simpleJsonTest ${JSONCPP_TEST_SOURCES} )

##
# Linker Command
##
target_link_libraries ( simpleJsonTest ${JSONCPP_TEST_LIBS} )

endif()
