cmake_minimum_required ( VERSION 2.8 )

if ( NOT DEFINED EVENT_MANAGER )
set ( EVENT_MANAGER 1 )

##
#   Paths file
##
include ( ${CMAKE_CURRENT_LIST_DIR}/../../build/CMakeLists.txt )

##
#   Includes
##
include_directories ( 
                      ${PLUGIN_LOADER_DIR}
                      ${JSONCPP_INCLUDE_DIR}
                      ${FRAMEWORK_COMMON_DIR}
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
set ( EVENT_MANAGER_SOURCES CEvent.cpp; 
                            CSubscriberMap.cpp;
                            CEventManager.cpp;
    ) 

prepend_path ( EVENT_MANAGER_SOURCES ${CMAKE_CURRENT_LIST_DIR} )

##
#   Libs
##
set ( EVENT_MANAGER_LIBS log4cxx;
    )
##
# Targets
##
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
add_library ( eventmanager SHARED ${EVENT_MANAGER_SOURCES} )

##
# Linker Command
##
target_link_libraries ( eventmanager ${EVENT_MANAGER_LIBS} )

endif()
