#include "CPluginLoader.h"

#include "IPluggable.h"
#include <dlfcn.h>

namespace framework
{
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
    int CPluginLoader::createPlugin ( const std::string& ar_library, 
                                      IPluggable*& apr_plugin )
    {
        int l_ret = 0;
        
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
            l_ret = -1;
        }
        else
        {
            //Now load the symbol
            tPluginSymbol l_loader = reinterpret_cast<tPluginSymbol>
                                        ( dlsym ( lp_handle, "createInstance" ) );
            
            if ( NULL != ( lp_errors = dlerror() ) )
            {
                LOG4CXX_ERROR ( m_logger, lp_errors );
                l_ret = -1;
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
};
