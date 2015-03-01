cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED SHADER_MANAGER_PLUGIN )
set ( SHADER_MANAGER_PLUGIN 1 )

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
                      ${SHADER_MANAGER_DIR}
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
                      ${CONTAINER_DIR}
                      ${RENDERABLE_DIR}
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
##Config
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/shaders.json.in
                 ${CMAKE_CURRENT_LIST_DIR}/shaders.json )

##
#   Source Files
##
set ( SHADER_MANAGER_PLUGIN_SOURCES 
        CShaderManagerPlugin.cpp; 
        CShaderDB.cpp; ) 

prepend_path ( SHADER_MANAGER_PLUGIN_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( SHADER_MANAGER_PLUGIN_LIBS log4cxx;
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
add_library ( shadermanagerplugin SHARED ${SHADER_MANAGER_PLUGIN_SOURCES} )

add_definitions ( -DGL_GLEXT_PROTOTYPES )

##
# Linker Command
##
target_link_libraries ( shadermanagerplugin ${SHADER_MANAGER_PLUGIN_LIBS} )

endif()
