cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED GL_MANAGER )
set ( GL_MANAGER 1 )

cmake_policy ( SET CMP0015 NEW )
##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${EVENT_MANAGER_DIR}
                      ${GLSDK_FREEGLUT_INCLUDE_DIR}
                      ${GLSDK_GLUTIL_INCLUDE_DIR}
                      ${GLSDK_GLFW_INCLUDE_DIR}
                      ${GLSDK_GLMESH_INCLUDE_DIR}
                      ${GLSDK_GLIMG_INCLUDE_DIR}
                      ${GLSDK_GLLOAD_INCLUDE_DIR}
                      ${BOOST_INCLUDE_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( 
                    ${JSONCPP_LIB_DIR}
                    ${EVENT_MANAGER_DIR}
                    ${GLSDK_FREEGLUT_LIB_DIR}
                    ${GLSDK_GLUTIL_LIB_DIR}
                    ${GLSDK_GLFW_LIB_DIR}
                    ${GLSDK_GLMESH_LIB_DIR}
                    ${GLSDK_GLIMG_LIB_DIR}
                    ${GLSDK_GLLOAD_LIB_DIR}
                    ${BOOST_LIB_DIR}
                 )

##
#   Source Files
##
set ( GL_MANAGER_SOURCES CGLManager.cpp; ) 

prepend_path ( GL_MANAGER_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( GL_MANAGER_LIBS log4cxx;
                      freeglut;                   
                      glutil;
                      glfw;
                      glmesh;
                      glimg;
                      glload;
                      GL
                      GLU
                      eventmanager;
                      boost_thread;
                      boost_system;
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( glmanagerplugin SHARED ${GL_MANAGER_SOURCES} )

##
# Linker Command
##
target_link_libraries ( glmanagerplugin ${GL_MANAGER_LIBS} )

endif()
