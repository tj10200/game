#ifndef CPluginLoader_h
#define CPluginLoader_h

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <vector>
#include <stdint.h>
#include "log4cxx/logger.h"
#include "IPluggable.h"

namespace framework
{
    /** Forward declaration for the event object **/
    class CEvent;

    typedef IPluggable* (*tPluginSymbol) ();

    /**
     * Class used to load and manage plugins
     */    
    class CPluginLoader
    {
        public:
            typedef std::unordered_set < IPluggable*, IPluggable::SPluggableHash, IPluggable::SPluggableEqual > tPluginSet;

            struct SLoadedPlugins
            {
                uint64_t m_handle;
                tPluginSet m_loadedPlugins;
            };

            typedef std::unordered_map < std::string, SLoadedPlugins* > tLibraryMap;
       
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
             * Destroys all plugin instances for a library
             *
             * @param ar_library - the plugin library to destroy
             * @return bool - true if handle found and destroyed
             */
            bool destroyPlugins ( const std::string& ar_library );
        
            /**
             * Destroys a plugin instance for a library. will
             * unload the library on the last instance
             *
             * @param ar_library - the plugin library to destroy
             * @param ap_inst - the plugin instance
             * @return bool - true if handle found and destroyed
             */
            bool destroyPlugin ( const std::string& ar_library,
                                  IPluggable* ap_inst );
        
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

            /** Getter for the plugin set **/
            const tPluginSet& getPlugins();

            /**
             * Get a plugin using the ID
             *
             * @param a_id - the id of the plugin to find
             * @param a_instId - the id of the plugin instance to find
             * @param apr_plugin - the plugin pointer to populate if found
             * @return bool - true if found
             */
            bool getPlugin ( const uint32_t a_id,
                             const uint32_t a_instId,
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

            /** The logger object **/
            log4cxx::LoggerPtr m_logger;

            /** The idle Callback Event object **/
            CEvent* mp_idleCallbackEvent;
            
            /** Global list of loaded plugins **/
            tPluginSet m_plugins;
    };
};

#endif
