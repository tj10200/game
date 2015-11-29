cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED SCENE_MANAGER_PLUGIN )
set ( SCENE_MANAGER_PLUGIN 1 )

cmake_policy ( SET CMP0015 NEW )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )
include ( ${PLUGIN_LOADER_DIR}/CMakeLists.txt )
include ( ${SHADER_MANAGER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                      ${SCENE_MANAGER_DIR}
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${EVENT_MANAGER_DIR}
                      ${RENDERABLE_PLUGIN_DIR}
                      ${CONTAINER_DIR}
                      ${RENDERABLE_DIR}
                      ${PLUGIN_LOADER_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( 
                    ${JSONCPP_LIB_DIR}
                    ${EVENT_MANAGER_DIR}
                 )
##Config
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/scenes.json.in
                 ${CMAKE_CURRENT_LIST_DIR}/scenes.json )

##
#   Source Files
##
set ( SCENE_MANAGER_PLUGIN_SOURCES 
        CSceneManagerPlugin.cpp;
        CSceneDB.cpp; ) 

prepend_path ( SCENE_MANAGER_PLUGIN_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( SCENE_MANAGER_PLUGIN_LIBS 
        log4cxx;
        eventmanager;
        pluginloader
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( scenemanagerplugin SHARED ${SCENE_MANAGER_PLUGIN_SOURCES} )

##
# Linker Command
##
target_link_libraries ( scenemanagerplugin ${SCENE_MANAGER_PLUGIN_LIBS} )

endif()
