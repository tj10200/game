cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED RENDERABLE_PLUGIN )
set ( RENDERABLE_PLUGIN 1 )

cmake_policy ( SET CMP0015 NEW )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )

##
#   Includes
##
message ( "Framework dir = ${FRAMEWORK_COMMON_DIR}")
include_directories ( 
                      ${RENDERABLE_DIR}
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${EVENT_MANAGER_DIR}
                      ${GL_MANAGER_DIR}
                      ${GLSDK_FREEGLUT_INCLUDE_DIR}
                      ${GLSDK_GLUTIL_INCLUDE_DIR}
                      ${GLSDK_GLFW_INCLUDE_DIR}
                      ${GLSDK_GLMESH_INCLUDE_DIR}
                      ${GLSDK_GLIMG_INCLUDE_DIR}
                      ${GLSDK_GLLOAD_INCLUDE_DIR}
                      ${FRAMEWORK_COMMON_DIR}
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
                 )

##
#   Source Files
##
set ( RENDERABLE_PLUGIN_SOURCES 
        IRenderable.cpp
    )

prepend_path ( RENDERABLE_PLUGIN_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( RENDERABLE_PLUGIN_LIBS log4cxx;
                      freeglut;                   
                      glutil;
                      glfw;
                      glmesh;
                      glimg;
                      glload;
                      GL
                      GLU
                      eventmanager;
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( simpletriangleplugin SHARED ${RENDERABLE_PLUGIN_SOURCES} )

add_definitions ( -DGL_GLEXT_PROTOTYPES )

##
# Linker Command
##
target_link_libraries ( simpletriangleplugin ${RENDERABLE_PLUGIN_LIBS} )

endif()
