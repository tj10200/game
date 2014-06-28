#ifndef CPluginLoader_h
#define CPluginLoader_h

#include <string>
#include <map>
#include <vector>
#include <stdint.h>
#include "log4cxx/logger.h"

namespace framework
{
    /** Forward declaration for the pluggin object **/
    class IPluggable;

    typedef IPluggable* (*tPluginSymbol) ();
        
    /**
     * Class used to load and manage plugins
     */    
    class CPluginLoader
    {
        public:
       
            /**
             * Constructor
             */
            CPluginLoader();

            /**
             * Destructor
             */
            virtual ~CPluginLoader();

            /**
             * Loads the library and creates a new object instance
             *
             * @param ar_library - the library name to load
             * @param apr_plugin - the plugin instance to populate
             * @return int - 0 on success. -1 otherwise 
             */
            int createPlugin ( const std::string& ar_library,
                               IPluggable*& apr_plugin );
        
            /**
             * Loads config for all plugins
             * 
             * @param ar_file - the config file to load
             * @return bool - true if all plugins successfully configured
             */
            virtual bool loadConfig ( std::string& ar_file );

        private:
            typedef std::map < const std::string, const uint64_t > tLibraryMap;
            typedef std::vector < IPluggable* > tPluginVector;
            /** Map containing all loaded libraries with handles**/
            tLibraryMap m_handles;

            /** Vector containing all loaded plugins **/
            tPluginVector m_plugins;

            /** The logger object **/
            log4cxx::LoggerPtr m_logger;
    };
};

#endif
