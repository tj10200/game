#include "CKeyboardPlugin.h"
#include "CEventManager.h"

#include "json/json.h"
#include <GL/freeglut.h>
//#include <GL/glext.h>

#include "GLManagerEvents.h"

namespace framework
{
    const uint8_t ESCAPE_KEY = 27;
    const uint32_t CKeyboardPlugin::PLUGIN_ID = 1405736502;

    //-----------------------------------------------------------------------//
    CKeyboardPlugin::CKeyboardPlugin()
    :   IPluggable("CKeyboardPlugin", PLUGIN_ID),
        m_leaveMainLoop ( false )
    {}

    //-----------------------------------------------------------------------//
    CKeyboardPlugin::~CKeyboardPlugin()
    {}

    //-----------------------------------------------------------------------//
    bool CKeyboardPlugin::loadConfig ( const Json::Value& ar_node )
    {
        bool l_ret = true;

        LOG4CXX_INFO ( m_logger, "Load Config" );
        
        Json::Value l_keyboardNode;

        if ( false == ar_node["keyboard"].isObject() )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to locate the keyboard element" );
            l_ret = false;
        }
        else
        {
            l_keyboardNode = ar_node["keyboard"]; 
        }

        if ( true == l_ret )
        {
            if ( false == l_keyboardNode.isMember ( "keyboard_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate keboard event subscriber name" );
                l_ret = false;
            }
            else
            {
                CEventManager* lp_eventManager = CEventManager::sGetInstance();
                
                lp_eventManager->registerEvent ( l_keyboardNode["keyboard_event"].asString().c_str(),
                                                 this,
                                                 sKeyboardCallback,
                                                 mp_keyboardCallbackEvent 
                                               );

            }
        }

        if ( true == l_ret )
        {
            if ( false == l_keyboardNode.isMember ( "window_close_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate keboard window close event subscriber name" );
                l_ret = false;
            }
            else
            {
                CEventManager* lp_eventManager = CEventManager::sGetInstance();
                
                lp_eventManager->registerEvent ( l_keyboardNode["window_close_event"].asString().c_str(),
                                                 this,
                                                 sCloseCallback,
                                                 mp_closeCallbackEvent 
                                               );

            }
        }

        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CKeyboardPlugin::start()
    {
    }
    
    //-----------------------------------------------------------------------//
    void CKeyboardPlugin::stop()
    {
        LOG4CXX_INFO ( m_logger, "Stop" );
        m_leaveMainLoop = true;
    }

    //-----------------------------------------------------------------------//
    void CKeyboardPlugin::sKeyboardCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CKeyboardPlugin*>(ap_instance)->keyboardCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CKeyboardPlugin::keyboardCallback ( void* ap_data )
    {
        SKeyboardEvent* lp_eventData = reinterpret_cast<SKeyboardEvent*>(ap_data);

        if ( ESCAPE_KEY == lp_eventData->m_key )
        {
            m_leaveMainLoop = true;
            glutLeaveMainLoop();
        }
    }

    //-----------------------------------------------------------------------//
    void CKeyboardPlugin::sCloseCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CKeyboardPlugin*>(ap_instance)->closeCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CKeyboardPlugin::closeCallback ( void* ap_data )
    {
        //Window gone. close down
        m_leaveMainLoop = true;
        glutLeaveMainLoop();
    }

    //-----------------------------------------------------------------------//
    bool CKeyboardPlugin::getLeaveMainLoopFlag()
    {
        return m_leaveMainLoop;
    }
};

//-----------------------------------------------------------------------//
extern "C"
{
    framework::IPluggable* createInstance()
    {
        return new framework::CKeyboardPlugin;
    }
}
