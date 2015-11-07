cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED RENDERABLE_PLUGIN )
set ( RENDERABLE_PLUGIN 1 )

cmake_policy ( SET CMP0015 NEW )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )
include ( ../CMakeLists.txt )

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
                      ${RENDERABLE_DIR}
                      ${CONTAINER_DIR}
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
                      renderablecommon
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( simpletriangleplugin SHARED 
                ${RENDERABLE_DIR}/IRenderable.cpp
                ${CMAKE_CURRENT_LIST_DIR}/CSimpleTrianglePlugin.cpp )
add_library ( simpleanimatabletriangleplugin SHARED 
                ${RENDERABLE_DIR}/IRenderable.cpp
                ${CMAKE_CURRENT_LIST_DIR}/CSimpleAnimatableTrianglePlugin.cpp )

add_definitions ( -DGL_GLEXT_PROTOTYPES )

##
# Linker Command
##
target_link_libraries ( simpletriangleplugin ${RENDERABLE_PLUGIN_LIBS} )
target_link_libraries ( simpleanimatabletriangleplugin ${RENDERABLE_PLUGIN_LIBS} )

endif()
