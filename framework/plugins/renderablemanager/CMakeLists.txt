cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED RENDERABLE_MANAGER )
set ( RENDERABLE_MANAGER_ 1 )

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
                      ${RENDERABLE_DIR}
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${EVENT_MANAGER_DIR}
                      ${FRAMEWORK_COMMON_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( 
                    ${JSONCPP_LIB_DIR}
                    ${EVENT_MANAGER_DIR}
                 )

##
#   Source Files
##
set ( RENDERABLE_MANAGER_SOURCES 
        CRenderableManagerPlugin.cpp; 
    )

prepend_path ( RENDERABLE_MANAGER_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( RENDERABLE_MANAGER_LIBS log4cxx;
                      eventmanager;
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( simpletriangleplugin SHARED ${RENDERABLE_MANAGER_SOURCES} )

add_definitions ( -DGL_GLEXT_PROTOTYPES )

##
# Linker Command
##
target_link_libraries ( simpletriangleplugin ${RENDERABLE_MANAGER_LIBS} )

endif()
