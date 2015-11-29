#include "CPluginLoader.h"

#include "IPluggable.h"
#include "CLookupPluggable.h"
#include "json/json.h"
#include <dlfcn.h>
#include <fstream>

#include "CEventManager.h"
#include "CEvent.h"

#include "CGLManager.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    CPluginLoader* CPluginLoader::getInstance()
    {
        static CPluginLoader ls_loader;
        
        return (&ls_loader);
    }

    //-----------------------------------------------------------------------//
    bool CPluginLoader::createPlugin ( const std::string& ar_library, 
                                      IPluggable*& apr_plugin )
    {
        bool l_ret = true;
        
        char* lp_errors = NULL;
        void* lp_handle = 0;
        SLoadedPlugins* lp_newInstance = NULL;

        tLibraryMap::iterator l_find = m_handles.find ( ar_library );

        //First clear any errors
        dlerror();

        if ( l_find != m_handles.end() )
        {
            //Not a new library. Load the symbol using the found handle
            lp_newInstance = l_find->second;
            lp_handle = (void *)(lp_newInstance->m_handle);
        }
        else
        {
            //Open up the library
            lp_handle = dlopen ( ar_library.c_str(), RTLD_LAZY );

            lp_newInstance = new SLoadedPlugins;
            lp_newInstance->m_handle = (uint64_t)lp_handle;

            //Library either found or opened successfully
            m_handles.insert ( make_pair ( ar_library, lp_newInstance ) );
            
        }        

        if ( NULL != ( lp_errors = dlerror() ) )
        {
            LOG4CXX_ERROR ( m_logger, lp_errors );
            l_ret = false;
        }
        else
        {
            //Now load the symbol
            tPluginSymbol l_loader = reinterpret_cast<tPluginSymbol>
                                        ( dlsym ( lp_handle, "createInstance" ) );
            
            if ( NULL != ( lp_errors = dlerror() ) )
            {
                LOG4CXX_ERROR ( m_logger, lp_errors );
                l_ret = false;
            }
            else
            {
                //Everything loaded correctly. Create the plugin
                apr_plugin = (*l_loader)();
                
                //And save the pointer for later
                while ( false == lp_newInstance->m_loadedPlugins.insert ( apr_plugin ).second )
                {
                    apr_plugin->setInstanceId( apr_plugin->getInstanceId() + 1 );
                }
                m_plugins.insert ( apr_plugin );
            }
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CPluginLoader::destroyPlugins ( const std::string& ar_library )
    {
        bool l_ret = true;

        tLibraryMap::iterator l_find = m_handles.find ( ar_library );

        //First clear any errors
        dlerror();

        if ( l_find != m_handles.end() )
        {
            //Not a new library. Load the symbol using the found handle
            SLoadedPlugins* lp_plugins = l_find->second;
            void* lp_handle = reinterpret_cast<void*>(lp_plugins->m_handle);

            tPluginSet::iterator l_iter = lp_plugins->m_loadedPlugins.begin();

            while ( l_iter != lp_plugins->m_loadedPlugins.end() )
            {
                m_plugins.erase ( (*l_iter) );

                delete ( *l_iter );
                ++l_iter;
            }

            dlclose ( lp_handle );

            m_handles.erase ( l_find );
        }
        else
        {
            l_ret = false;
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CPluginLoader::destroyPlugin ( const std::string& ar_library,
                                        IPluggable* ap_inst )
    {
        bool l_ret = true;

        if ( NULL == ap_inst )
        {
            return false;
        }

        tLibraryMap::iterator l_find = m_handles.find ( ar_library );

        //First clear any errors
        dlerror();

        if ( l_find != m_handles.end() )
        {
            //Not a new library. Load the symbol using the found handle
            SLoadedPlugins* lp_plugins = l_find->second;
            void* lp_handle = reinterpret_cast<void*>(lp_plugins->m_handle);

            tPluginSet::iterator l_findplugin = lp_plugins->m_loadedPlugins.find ( ap_inst );

            if ( l_findplugin != lp_plugins->m_loadedPlugins.end() )
            {
                tPluginSet::iterator l_globalplugin = m_plugins.find ( ap_inst );
                if ( l_globalplugin != m_plugins.end() )
                {
                    m_plugins.erase( l_globalplugin );
                }

                delete ( *l_findplugin );
                lp_plugins->m_loadedPlugins.erase ( l_findplugin );
            }

            if ( 0 == lp_plugins->m_loadedPlugins.size() )
            {
                dlclose ( lp_handle );

                m_handles.erase ( l_find );
            }
        }
        else
        {
            l_ret = false;
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CPluginLoader::loadConfig ( std::string& ar_file )
    {
        bool l_ret = true;

        Json::Value l_rootNode; 
        Json::Reader l_reader;
        std::ifstream l_testFile ( ar_file.c_str(), std::ifstream::binary);

        if ( false == l_reader.parse( l_testFile, l_rootNode ) )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to parse Json file" );
           
            l_ret = false; 
        }

        
        /*
            Loop through the config file and load the plugins in the order they are listed
            . Here is an example config for the gl manager plugin
            "plugins" :
            [
                {
                    "library" : "/home/tj/game/framework/glmanager/libglmanagerplugin.so",
                    "manager" : 
                    {
                        "window_width"  : 500,
                        "window_height" : 300,
                        "window_horizontal_start" : 200,
                        "window_vertical_start" :   300,
                        "display_mode" :
                        [
                            "GLUT_DOUBLE",
                            "GLUT_ALPHA",
                            "GLUT_DEPTH",
                            "GLUT_STENCIL"
                        ]
                    }
                }
            ]
        
        */
        if ( true == l_ret )
        {
            if ( true == l_rootNode["plugins"].isArray() )
            {
                for ( int i = 0; i < l_rootNode["plugins"].size() && l_ret; i++ )
                {
                    Json::Value l_plugin = l_rootNode["plugins"][i];
                    IPluggable* lp_plugin = NULL;
                
                    if ( true == l_plugin.isMember( "library" ) )
                    {
                        LOG4CXX_INFO ( m_logger, "Loading library : " <<
                                                 l_plugin["library"].asString() );
                        
                        if ( true == ( l_ret = createPlugin( l_plugin["library"].asString(),
                                                             lp_plugin ) ) )
                        {
                            LOG4CXX_INFO ( m_logger, "Configuring object : " <<
                                                     lp_plugin->getName() );
                            l_ret = lp_plugin->loadConfig ( l_plugin );
                        }
                        else
                        {
                            LOG4CXX_ERROR ( m_logger, "Failed to load plugin instance" );
                        }
                    }
                     
                }
            }
            else
            {
                LOG4CXX_ERROR ( m_logger, "Plugins root node not present" );
                l_ret = false;
            }
        }

        if ( true == l_rootNode.isMember( "idle_event" ) )
        {
            LOG4CXX_INFO ( m_logger, "Configuring Idle Event : " << l_rootNode["idle_event"].asString().c_str()  );

            CEventManager* lp_eventManager = CEventManager::sGetInstance();
                
            lp_eventManager->registerEvent ( l_rootNode["idle_event"].asString().c_str(),
                                             this,
                                             sIdleCallback,
                                             mp_idleCallbackEvent 
                                           );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CPluginLoader::startPlugins()
    {
        tPluginSet::iterator l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            IPluggable* lp_plugin = (*l_pluginIter);

            //Start the plugin
            lp_plugin->start();

            ++l_pluginIter;
        }
    }

    //-----------------------------------------------------------------------//
    void CPluginLoader::stopPlugins()
    {
        tPluginSet::iterator l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            IPluggable* lp_plugin = (*l_pluginIter);

            //Stop the plugin
            lp_plugin->stop();

            ++l_pluginIter;
        }
    }

    //-----------------------------------------------------------------------//
    const CPluginLoader::tPluginSet& CPluginLoader::getPlugins()
    {
        return m_plugins;
    }

    //-----------------------------------------------------------------------//
    bool CPluginLoader::getPlugin ( const uint32_t a_id,
                                    const uint32_t a_instId,
                                    IPluggable*& apr_plugin )
    {
        bool l_ret = false;
    
        CLookupPluggable l_lookup;
        l_lookup.setId ( a_id );
        l_lookup.setInstanceId ( a_instId );

        tPluginSet::iterator l_pluginIter = m_plugins.find( &l_lookup );
        
        if ( l_pluginIter != m_plugins.end() )
        {
            apr_plugin = (*l_pluginIter);
            l_ret = true;                
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    CPluginLoader::CPluginLoader()
    : m_logger ( log4cxx::Logger::getLogger ( "CPluginLoader" ) )
    {}

    //-----------------------------------------------------------------------//
    CPluginLoader::~CPluginLoader()
    {
        tPluginSet::iterator l_pluginIter;
        IPluggable* lp_plugin = NULL;

        l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            lp_plugin = (*l_pluginIter);
            lp_plugin->stop ();

            delete lp_plugin;

            ++l_pluginIter;
        }

        //All plugins are destroyed
        // Now unload the libraries
        tLibraryMap::iterator l_handleIter = m_handles.begin();
        SLoadedPlugins* lp_current = NULL;

        while ( l_handleIter != m_handles.end() )
        {
            lp_current = l_handleIter->second;

            void* lp_handle = (void *)(lp_current->m_handle);
        
            dlclose ( lp_handle );

            delete lp_current;

            ++l_handleIter;
        }
        
    }

    //-----------------------------------------------------------------------//
    void CPluginLoader::sIdleCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CPluginLoader*>(ap_instance)->idleCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CPluginLoader::idleCallback ( void* ap_data )
    {
        updatePlugins();
    }

    //-----------------------------------------------------------------------//
    void CPluginLoader::updatePlugins()
    {
        IPluggable* lp_glmanager = NULL;
        getPlugin (  CGLManager::GL_MANAGER_ID, 0, lp_glmanager );

        float l_elapsedSeconds = static_cast < CGLManager* >(lp_glmanager)->getElapsedTime ();

        tPluginSet::iterator l_pluginIter;
        IPluggable* lp_plugin = NULL;

        l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            lp_plugin = (*l_pluginIter);
            lp_plugin->update ( l_elapsedSeconds );

            ++l_pluginIter;
        }
    }

};

