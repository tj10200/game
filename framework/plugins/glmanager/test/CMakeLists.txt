cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED PLUGGABLE_TEST )
set ( PLUGGABLE_TEST 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )
include ( ${GL_MANAGER_DIR}/CMakeLists.txt )
include ( ${PLUGIN_LOADER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( ${GL_MANAGER_DIR}
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
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
set ( GL_MANAGER_TEST_SOURCES managerTest.cpp )
prepend_path ( GL_MANAGER_TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( GL_MANAGER_TEST_LIBS  pluginloader;
                        json_linux-gcc-4.6_libmt;
                        gtest;
                        gtest_main;
                        pthread  )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
set ( CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_executable ( managerTest ${GL_MANAGER_TEST_SOURCES} )

##
# Linker Command
##
target_link_libraries ( managerTest ${GL_MANAGER_TEST_LIBS} )

endif()
