cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED DISPLAY_PLUGIN )
set ( DISPLAY_PLUGIN 1 )

cmake_policy ( SET CMP0015 NEW )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )
include ( ${RENDERABLE_DIR}/CMakeLists.txt )
include ( ${SHADER_MANAGER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
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
                      ${RENDERABLE_PLUGIN_DIR}
                      ${SHADER_MANAGER_DIR}
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
set ( DISPLAY_PLUGIN_SOURCES CDisplayPlugin.cpp; ) 

prepend_path ( DISPLAY_PLUGIN_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( DISPLAY_PLUGIN_LIBS log4cxx;
                      freeglut;                   
                      glutil;
                      glfw;
                      glmesh;
                      glimg;
                      glload;
                      GL
                      GLU
                      eventmanager;
                      renderablecommon 
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( displayplugin SHARED ${DISPLAY_PLUGIN_SOURCES} )

add_definitions ( -DGL_GLEXT_PROTOTYPES )

##
# Linker Command
##
target_link_libraries ( displayplugin ${DISPLAY_PLUGIN_LIBS} )

endif()
