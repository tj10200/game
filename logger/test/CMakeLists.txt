cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED LOG4CXXP_TEST )
set ( LOG4CXX_TEST 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../build/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                      ${GTEST_INCLUDE_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( 
                   ${GTEST_LIB_DIR}
                 )

##
#   Source Files
##
set ( LOG4CXX_TEST_SOURCES simpleLoggerTest.cpp )

##
#   Libs
##
set ( LOG4CXX_TEST_LIBS log4cxx
                    
    )
##
# Targets
##
add_executable ( simpleLoggerTest ${LOG4CXX_TEST_SOURCES} )

##
# Linker Command
##
target_link_libraries ( simpleLoggerTest ${LOG4CXX_TEST_LIBS} )

endif()
