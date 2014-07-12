#include "CGLManager.h"

#include "json/json.h"
#include <fstream>
#include <GL/freeglut.h>
#include "CPluginLoader.h"
#include "CEventManager.h"
#include "CEvent.h"
#include "GLManagerEvents.h"

namespace framework
{
    const uint32_t CGLManager::GL_MANAGER_ID = 1404222126;

    //-----------------------------------------------------------------------//
    CGLManager::CGLManager()
    :   IPluggable ( "CGLManager", GL_MANAGER_ID ),
        m_windowWidth ( 0 ),
        m_windowHeight ( 0 ),
        m_windowStartHorizontal ( 0 ),
        m_windowStartVertical ( 0 ),        
        m_displayMode ( 0 ),
        mp_eventManager ( CEventManager::sGetInstance() ),
        mp_displayEvent ( NULL ),
        mp_reshapeEvent ( NULL ),
        mp_keyboardEvent ( NULL )
    {}

    //-----------------------------------------------------------------------//
    CGLManager::~CGLManager()
    {}

    //-----------------------------------------------------------------------//
    bool CGLManager::loadConfig ( const Json::Value& ar_node )
    {
        bool l_ret = true;
        
        Json::Value l_manager;

        if ( false == ar_node["manager"].isObject() )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to locate the manager element" );
            l_ret = false;
        }
        else
        {
            l_manager = ar_node["manager"]; 
        }
        
        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "window_width" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate window_width node" );
                l_ret = false;
            }
            else
            {
                m_windowWidth = l_manager["window_width"].asInt();
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "window_height" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate window_height node" );
                l_ret = false;
            }
            else
            {
                m_windowHeight = l_manager["window_height"].asInt();
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "window_horizontal_start" ) )
            {
                LOG4CXX_INFO ( m_logger, "Unable to locate window_horizontal start. Starting at 0" );
                m_windowStartHorizontal = 0;
            }
            else
            {
                m_windowStartHorizontal = l_manager["window_horizontal_start"].asInt();
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "window_vertical_start" ) )
            {
                LOG4CXX_INFO ( m_logger, "Unable to locate window_vertical start. Starting at 0" );
                m_windowStartVertical = 0;
            }
            else
            {
                m_windowStartVertical = l_manager["window_vertical_start"].asInt();
            }
        }

        if ( true == l_ret )
        {
            Json::Value l_displayMode;

            if ( false == l_manager["display_mode"].isArray() )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display_mode node." );
                l_ret = false;
            }
            else
            {
                l_displayMode = l_manager["display_mode"];

                for ( int i = 0; i < l_displayMode.size() && l_ret; i++ )
                {
                    if ( "GLUT_DOUBLE" == l_displayMode[i].asString() )
                    {
                        m_displayMode |= GLUT_DOUBLE;
                    }
                    else if ( "GLUT_ALPHA" == l_displayMode[i].asString() )
                    {
                        m_displayMode |= GLUT_ALPHA;
                    }
                    else if ( "GLUT_DEPTH" == l_displayMode[i].asString() )
                    {
                        m_displayMode |= GLUT_DEPTH;
                    }
                    else if ( "GLUT_STENCIL" == l_displayMode[i].asString() )
                    {
                        m_displayMode |= GLUT_STENCIL;
                    }
                    else
                    {
                        LOG4CXX_WARN ( m_logger, "Unknown display mode type encountered - " <<
                                                 l_displayMode[i].asString() );
                        l_ret = false;
                                                
                    }
                }
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "display_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display event publish name" );
                l_ret = false;
            }
            else
            {
                mp_displayEvent = new CEvent ( l_manager["display_event"].asString().c_str() );
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "reshape_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate reshape event publish name" );
                l_ret = false;
            }
            else
            {
                mp_reshapeEvent = new CEvent ( l_manager["reshape_event"].asString().c_str() );
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_manager.isMember ( "keyboard_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate keyboard event publish name" );
                l_ret = false;
            }
            else
            {
                mp_keyboardEvent = new CEvent ( l_manager["keyboard_event"].asString().c_str() );
            }
        }


        return l_ret; 
    }

    //-----------------------------------------------------------------------//
    void CGLManager::start()
    {
        int l_argc = 0;
        char** lp_argv;

        glutInit ( &l_argc, lp_argv );

        glutInitDisplayMode (m_displayMode);
        glutInitContextVersion (3, 3);
        glutInitContextProfile(GLUT_CORE_PROFILE);

        #ifdef DEBUG
            glutInitContextFlags(GLUT_DEBUG);
        #endif

        glutInitWindowSize ( m_windowWidth, m_windowHeight); 

        glutInitWindowPosition (m_windowStartHorizontal, m_windowStartVertical);

        int l_window = glutCreateWindow ("Manager");

        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

        glutDisplayFunc(sDisplayFunction); 
        glutReshapeFunc(sReshapeFunction);
        glutKeyboardFunc(sKeyboardFunction);

        glutMainLoop();
    }

    //-----------------------------------------------------------------------//
    void CGLManager::stop()
    {}

    //-----------------------------------------------------------------------//
    void CGLManager::sDisplayFunction ()
    {
        CGLManager* lp_global = sGetLoadedInstance();

        if ( NULL != lp_global )
        {
            lp_global->displayFunction();
        }
    }

    //-----------------------------------------------------------------------//
    void CGLManager::displayFunction()
    {
        if ( NULL != mp_displayEvent )
        {
            mp_eventManager->publishEvent ( mp_displayEvent, 
                                            NULL );
                                            
        }
    }

    //-----------------------------------------------------------------------//
    void  CGLManager::sReshapeFunction ( int a_width, int a_height )
    {
        CGLManager* lp_global = sGetLoadedInstance();

        if ( NULL != lp_global )
        {
            lp_global->reshapeFunction( a_width, a_height );
        }
    }

    //-----------------------------------------------------------------------//
    void CGLManager::reshapeFunction( int a_width, int a_height )
    {
        if ( NULL != mp_reshapeEvent )
        {
            SReshapeEvent l_eventData;
            l_eventData.m_width     = a_width;
            l_eventData.m_height    = a_height;

            mp_eventManager->publishEvent ( mp_reshapeEvent, 
                                            (&l_eventData) );
        }
    }

    //-----------------------------------------------------------------------//
    void  CGLManager::sKeyboardFunction ( unsigned char a_key,
                                     int a_mouseX, int a_mouseY )
    {
        CGLManager* lp_global = sGetLoadedInstance();

        if ( NULL != lp_global )
        {
            lp_global->keyboardFunction ( a_key, a_mouseX, a_mouseY );
        }
    }

    //-----------------------------------------------------------------------//
    void CGLManager::keyboardFunction( unsigned char a_key,
                                       int a_mouseX,
                                       int a_mouseY )
    {
        if ( NULL != mp_keyboardEvent )
        {
            SKeyboardEvent l_eventData;
            l_eventData.m_key       = a_key;
            l_eventData.m_mouseX    = a_mouseX;
            l_eventData.m_mouseY    = a_mouseY;

            mp_eventManager->publishEvent ( mp_keyboardEvent,
                                            (&l_eventData ) );
        }
    }

    //-----------------------------------------------------------------------//
    CGLManager* CGLManager::sGetLoadedInstance()
    {
        static CPluginLoader* lsp_loader = CPluginLoader::getInstance();
        static IPluggable* lsp_return = NULL;

        if ( NULL == lsp_return )
        {
            lsp_loader->getPlugin ( GL_MANAGER_ID, lsp_return );
        }

        return static_cast<CGLManager*>(lsp_return);
    }

};

//-----------------------------------------------------------------------//
extern "C"
{
    framework::IPluggable* createInstance()
    {
        return new framework::CGLManager;
    }
}
