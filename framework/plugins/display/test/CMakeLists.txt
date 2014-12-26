cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED DISPLAY_TEST )
set ( DISPLAY_TEST 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../../build/CMakeLists.txt )
include ( ${PLUGIN_LOADER_DIR}/CMakeLists.txt )
include ( ${DISPLAY_PLUGIN_DIR}/CMakeLists.txt )
include ( ${KEYBOARD_PLUGIN_DIR}/CMakeLists.txt )
include ( ${GL_MANAGER_DIR}/CMakeLists.txt )
include ( ${KEYBOARD_PLUGIN_DIR}/CMakeLists.txt )

##
# Config files
##
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/pluginLoaderTestConfig.json.in
                 ${CMAKE_CURRENT_LIST_DIR}/pluginLoaderTestConfig.json )

##
#   Includes
##
include_directories ( ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${GTEST_INCLUDE_DIR}
                      ${GL_MANAGER_DIR}
                      ${GLSDK_FREEGLUT_INCLUDE_DIR}
                      ${GLSDK_GLUTIL_INCLUDE_DIR}
                      ${GLSDK_GLFW_INCLUDE_DIR}
                      ${GLSDK_GLMESH_INCLUDE_DIR}
                      ${GLSDK_GLIMG_INCLUDE_DIR}
                      ${GLSDK_GLLOAD_INCLUDE_DIR}
                      ${KEYBOARD_PLUGIN_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( ${JSONCPP_LIB_DIR}
                   ${GTEST_LIB_DIR}
                    ${GLSDK_FREEGLUT_LIB_DIR}
                    ${GLSDK_GLUTIL_LIB_DIR}
                    ${GLSDK_GLFW_LIB_DIR}
                    ${GLSDK_GLMESH_LIB_DIR}
                    ${GLSDK_GLIMG_LIB_DIR}
                    ${GLSDK_GLLOAD_LIB_DIR}
                 )

##
#   Source Files
##
set ( PLUGIN_TEST_SOURCES simplePluginTest.cpp )
prepend_path ( PLUGIN_TEST_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( PLUGIN_TEST_LIBS  pluginloader;
                        json_linux-gcc-4.6_libmt;
                        gtest;
                        gtest_main;
                        pthread;
                        GL;
                        GLU;
                        glut
                        glutil;
                        glfw;
                        glmesh;
                        glimg;
                        glload;
                        boost_system 
                        keyboardplugin
                      )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
set ( CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )

add_executable ( pluginTest ${PLUGIN_TEST_SOURCES} )

##
# Linker Command
##
target_link_libraries ( pluginTest ${PLUGIN_TEST_LIBS} )

endif()
