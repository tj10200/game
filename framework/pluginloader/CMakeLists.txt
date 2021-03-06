cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED PLUGIN_LOADER )
set ( PLUGIN_LOADER 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../build/CMakeLists.txt )
include ( ${EVENT_MANAGER_DIR}/CMakeLists.txt )
include ( ${GL_MANAGER_DIR}/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                        ${JSONCPP_INCLUDE_DIR}
                        ${EVENT_MANAGER_DIR}
                        ${GL_MANAGER_DIR}
                    )

## 
#   Lib Directories
##
link_directories ( 
                    ${JSONCPP_LIB_DIR}
                 )

##
#   Source Files
##
set ( PLUGIN_LOADER_SOURCES CPluginLoader.cpp;
                            IPluggable.cpp )

prepend_path ( PLUGIN_LOADER_SOURCES ${CMAKE_CURRENT_LIST_DIR} )


##
#   Libs
##
set ( PLUGIN_LOADER_LIBS log4cxx;
                         dl
                         json_linux-gcc-4.6_libmt;
                         glmanagerplugin;
                    
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( pluginloader SHARED ${PLUGIN_LOADER_SOURCES} )

##
# Linker Command
##
target_link_libraries ( pluginloader ${PLUGIN_LOADER_LIBS} )

endif()
