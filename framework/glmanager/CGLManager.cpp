#include "CGLManager.h"

#include "json/json.h"
#include <fstream>
#include <GL/freeglut.h>


namespace framework
{
    //-----------------------------------------------------------------------//
    CGLManager::CGLManager()
    :   IPluggable ( "CGLManager" ),
        m_windowWidth ( 0 ),
        m_windowHeight ( 0 ),
        m_windowStartHorizontal ( 0 ),
        m_windowStartVertical ( 0 ),        
        m_displayMode ( 0 )
    {}

    //-----------------------------------------------------------------------//
    CGLManager::~CGLManager()
    {}

    //-----------------------------------------------------------------------//
    bool CGLManager::loadConfig ( const Json::Value& ar_node )
    {
        bool l_ret = true;
        
        Json::Value l_manager;

        if ( "" == ar_node["manager"].asString() )
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
            if ( "" == l_manager["window_width"].asString() )
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
            if ( "" == l_manager["window_height"].asString() )
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
            if ( "" == l_manager["window_horizontal_start"].asString() )
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
            if ( "" == l_manager["window_vertical_start"].asString() )
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

            if ( "" == l_manager["display_mode"].asString() )
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

        /*
        if(glext_ARB_debug_output)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
            glDebugMessageCallbackARB(DebugFunc, (void*)15);
        }
        */

        //glutDisplayFunc(display); 
        //glutReshapeFunc(reshape);
        //glutKeyboardFunc(keyboard);

        glutMainLoop();
    }

    //-----------------------------------------------------------------------//
    void CGLManager::stop()
    {}

};
