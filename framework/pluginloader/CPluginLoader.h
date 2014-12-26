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
    class CEvent;

    typedef IPluggable* (*tPluginSymbol) ();

    /**
     * Class used to load and manage plugins
     */    
    class CPluginLoader
    {
        public:
            typedef std::map < const std::string, const uint64_t > tLibraryMap;
            typedef std::vector < IPluggable* > tPluginVector;
       
            /**
             * Creates or retrieves the instance of the loader object
             */
            static CPluginLoader* getInstance();

            /**
             * Loads the library and creates a new object instance
             *
             * @param ar_library - the library name to load
             * @param apr_plugin - the plugin instance to populate
             * @return bool - true on success. false otherwise 
             */
            bool createPlugin ( const std::string& ar_library,
                                IPluggable*& apr_plugin );
        
            /**
             * Loads config for all plugins
             * 
             * @param ar_file - the config file to load
             * @return bool - true if all plugins successfully configured
             */
            virtual bool loadConfig ( std::string& ar_file );

            /**
             * Starts all plugins
             */
            void startPlugins();
            
            /**
             * Stops all plugins
             */
            void stopPlugins();

            /** Getter for the plugin vector **/
            const tPluginVector& getPlugins();
        
            /**
             * Get a plugin using the ID
             *
             * @param a_id - the id of the plugin to find
             * @param apr_plugin - the plugin pointer to populate if found
             * @return bool - true if found
             */
            bool getPlugin ( const uint32_t a_id,
                             IPluggable*& apr_plugin );

            /**
             * The idle callback function
             */
            static void sIdleCallback ( void* ap_instance, 
                                        void* ap_data );
            void idleCallback ( void* ap_data );

            /**
             * Updates the plugins with the current time
             *
             */
            void updatePlugins();

        private:
            
            /**
             * Constructor
             */
            CPluginLoader();

            /**
             * Destructor
             */
            virtual ~CPluginLoader();

            /**
             * Copy constructor
             */
            CPluginLoader ( const CPluginLoader& ar_copy )
            {}

            /**
             * Assignment operator
             */
            CPluginLoader& operator= ( const CPluginLoader& ar_right )
            { return *this; }
        
        private:
            /** Map containing all loaded libraries with handles**/
            tLibraryMap m_handles;

            /** Vector containing all loaded plugins **/
            tPluginVector m_plugins;

            /** The logger object **/
            log4cxx::LoggerPtr m_logger;

            /** The idle Callback Event object **/
            CEvent* mp_idleCallbackEvent;
            
    };
};

#endif
