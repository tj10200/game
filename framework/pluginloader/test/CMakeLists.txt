cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED PLUGGABLE_TEST )
set ( PLUGGABLE_TEST 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )
include ( ${PLUGIN_LOADER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( ${PLUGIN_LOADER_DIR}
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
set ( PLUGIN_LIB_SOURCES CPluggableStub.cpp )
prepend_path ( PLUGIN_LIB_SOURCES ${CMAKE_CURRENT_LIST_DIR} )
set ( PLUGIN_TEST_SOURCES simplePluginTest.cpp )
prepend_path ( PLUGIN_TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( PLUGIN_LIB_LIBS   json_linux-gcc-4.6_libmt;
                        pthread  )

set ( PLUGIN_TEST_LIBS  pluginloader;
                        json_linux-gcc-4.6_libmt;
                        gtest;
                        gtest_main;
                        pthread  )
##
# Targets
##
add_library ( pluginStub SHARED ${PLUGIN_LIB_SOURCES} )
add_executable ( pluginTest ${PLUGIN_TEST_SOURCES} )

##
# Linker Command
##
target_link_libraries ( pluginStub ${PLUGIN_LIB_LIBS} )
target_link_libraries ( pluginTest ${PLUGIN_TEST_LIBS} )

endif()
