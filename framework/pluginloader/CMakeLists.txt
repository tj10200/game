cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED PLUGIN_LOADER )
set ( PLUGIN_LOADER 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../build/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                    )

## 
#   Lib Directories
##
link_directories ( 
                 )

##
#   Source Files
##
set ( PLUGIN_LOADER_SOURCES CPluginLoader.cpp;
                            IPluggable.cpp )

##
#   Libs
##
set ( PLUGIN_LOADER_LIBS log4cxx;
                         dl
                    
    )
##
# Targets
##
add_library ( pluginloader SHARED ${PLUGIN_LOADER_SOURCES} )

##
# Linker Command
##
target_link_libraries ( pluginloader ${PLUGIN_LOADER_LIBS} )

endif()
