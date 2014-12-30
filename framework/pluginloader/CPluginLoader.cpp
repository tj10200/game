#include "CPluginLoader.h"

#include "IPluggable.h"
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

        tLibraryMap::iterator l_find = m_handles.find ( ar_library );

        //First clear any errors
        dlerror();

        if ( l_find != m_handles.end() )
        {
            //Not a new library. Load the symbol using the found handle
            lp_handle = reinterpret_cast<void*>(l_find->second);
        }
        else
        {
            //Open up the library
            lp_handle = dlopen ( ar_library.c_str(), RTLD_LAZY );

            //Library either found or opened successfully
            m_handles.insert ( make_pair ( ar_library, (uint64_t)lp_handle ) );
            
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
                m_plugins.push_back ( apr_plugin );
            }
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
        tPluginVector::iterator l_pluginIter = m_plugins.begin();

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
        tPluginVector::iterator l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            IPluggable* lp_plugin = (*l_pluginIter);

            //Stop the plugin
            lp_plugin->stop();

            ++l_pluginIter;
        }
    }

    //-----------------------------------------------------------------------//
    const CPluginLoader::tPluginVector& CPluginLoader::getPlugins()
    {
        return m_plugins;
    }

    //-----------------------------------------------------------------------//
    bool CPluginLoader::getPlugin ( const uint32_t a_id,
                                    IPluggable*& apr_plugin )
    {
        bool l_ret = false;
    
        tPluginVector::iterator l_pluginIter = m_plugins.begin();
        
        while ( l_pluginIter != m_plugins.end() &&
                false == l_ret )
        {
            IPluggable* lp_plugin = (*l_pluginIter);

            if ( lp_plugin->getId() == a_id )
            {
                //Plugin found. Set the flag
                apr_plugin = lp_plugin;
                l_ret = true;                
            }

            ++l_pluginIter;
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
        tPluginVector::iterator l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            IPluggable* lp_plugin = (*l_pluginIter);

            //Stop the plugin
            lp_plugin->stop();

            //And destroy it
            delete lp_plugin;
        
            ++l_pluginIter;
        }

        //All plugins are destroyed
        // Now unload the libraries
        tLibraryMap::iterator l_handleIter = m_handles.begin();

        while ( l_handleIter != m_handles.end() )
        {
            void* lp_handle = reinterpret_cast<void*>(l_handleIter->second);
        
            dlclose ( lp_handle );

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
        getPlugin (  CGLManager::GL_MANAGER_ID, lp_glmanager );

        float l_elapsedSeconds = static_cast < CGLManager* >(lp_glmanager)->getElapsedTime ();

        tPluginVector::iterator l_pluginIter = m_plugins.begin();

        while ( l_pluginIter != m_plugins.end() )
        {
            IPluggable* lp_plugin = (*l_pluginIter);

            //Start the plugin
            lp_plugin->update( l_elapsedSeconds );

            ++l_pluginIter;
        }
    }

};

