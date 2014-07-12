cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED GL_MANAGER )
set ( GL_MANAGER 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${EVENT_MANAGER_DIR}
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
set ( GL_MANAGER_SOURCES CGLManager.cpp; ) 

prepend_path ( GL_MANAGER_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( GL_MANAGER_LIBS log4cxx;
                      GL
                      glut;                   
                      eventmanager;
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
