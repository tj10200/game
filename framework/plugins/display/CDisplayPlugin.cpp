#include "CDisplayPlugin.h"
#include "CEventManager.h"

#include "json/json.h"
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>

#include "GLManagerEvents.h"

#include "CPluginLoader.h"
#include "IRenderable.h"

namespace framework
{
    const uint32_t DISPLAY_PLUGIN_ID = 1405198146;
    IPluggable * gp_renderable = NULL;

    //-----------------------------------------------------------------------//
    CDisplayPlugin::CDisplayPlugin()
    :   IPluggable("CDisplayPlugin", DISPLAY_PLUGIN_ID),
        m_windowSizeUniform(0),
        m_windowSize(0)
    {}

    //-----------------------------------------------------------------------//
    CDisplayPlugin::~CDisplayPlugin()
    {}

    //-----------------------------------------------------------------------//
    bool CDisplayPlugin::loadConfig ( const Json::Value& ar_node )
    {
        bool l_ret = true;

        LOG4CXX_INFO ( m_logger, "Load Config" );
        
        Json::Value l_displayNode;

        if ( false == ar_node["display"].isObject() )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to locate the manager element" );
            l_ret = false;
        }
        else
        {
            l_displayNode = ar_node["display"]; 
        }

        if ( true == l_ret )
        {
            if ( false == l_displayNode.isMember ( "display_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display event subscriber name" );
                l_ret = false;
            }
            else
            {
                CEventManager* lp_eventManager = CEventManager::sGetInstance();
                
                lp_eventManager->registerEvent ( l_displayNode["display_event"].asString().c_str(),
                                                 this,
                                                 sDisplayCallback,
                                                 mp_displayCallbackEvent 
                                               );
            }
        }
        
        if ( true == l_ret )
        {
            if ( false == l_displayNode.isMember ( "reshape_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display event subscriber name" );
                l_ret = false;
            }
            else
            {
                CEventManager* lp_eventManager = CEventManager::sGetInstance();
                
                lp_eventManager->registerEvent ( l_displayNode["reshape_event"].asString().c_str(),
                                                 this,
                                                 sReshapeCallback,
                                                 mp_reshapeCallbackEvent 
                                               );
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_displayNode.isMember ( "renderable_plugin" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to load renderable plugin used in display" );
                l_ret = false;
            }
            else
            {
                framework::CPluginLoader* lp_loader = CPluginLoader::getInstance();
                lp_loader->createPlugin ( l_displayNode["renderable_plugin"].asString(),
                                          gp_renderable );
            }   
        }
        
        //Now load the shader programs
        tShaderVec l_shaderVec;

        if ( true == l_ret )
        {
            if ( false == l_displayNode.isMember ( "vertex_shader" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display vertex shader node" );
                l_ret = false;
            }
            else
            {
                std::string l_vertexShader = l_displayNode["vertex_shader"].asString();
                l_shaderVec.push_back ( loadShader( GL_VERTEX_SHADER,
                                                    l_vertexShader ) );
            }
        }

        if ( true == l_ret )
        {
            if ( false == l_displayNode.isMember ( "frag_shader" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display fragment shader node" );
                l_ret = false;
            }
            else
            {
                std::string l_fragShader = l_displayNode["frag_shader"].asString();
                l_shaderVec.push_back ( loadShader( GL_FRAGMENT_SHADER,
                                                    l_fragShader ) );
            }
        }
        
        if ( true == l_ret )
        {
            m_shaderProgramHandle = createProgram ( l_shaderVec );
            
            m_windowSizeUniform = glGetUniformLocation ( m_shaderProgramHandle, "windowSize" );            

        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::start()
    {
        gp_renderable->start();
    }
    
    //-----------------------------------------------------------------------//
    void CDisplayPlugin::stop()
    {
        LOG4CXX_INFO ( m_logger, "Stop" );
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::sDisplayCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CDisplayPlugin*>(ap_instance)->displayCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::displayCallback ( void* ap_data )
    {
        glClearColor(0.4f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_shaderProgramHandle);
        glUniform1f ( m_windowSizeUniform, m_windowSize );

        static_cast<IRenderable*>(gp_renderable)->render(); 

        glUseProgram(0);

        glutSwapBuffers();
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::sReshapeCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CDisplayPlugin*>(ap_instance)->reshapeCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::reshapeCallback ( void* ap_data )
    {
        SReshapeEvent* lp_event = reinterpret_cast < SReshapeEvent*>(ap_data);

        glViewport(0, 0, lp_event->m_width, lp_event->m_height);

        m_windowSize = lp_event->m_width;
    }

    //-----------------------------------------------------------------------//
    GLuint CDisplayPlugin::loadShader ( GLenum a_shaderType,
                                        std::string& ar_shaderFile )
    {
        GLuint l_return = GL_FALSE;

        LOG4CXX_INFO ( m_logger, "Loading Shader from File: " << ar_shaderFile );
        std::ifstream l_shaderFile ( ar_shaderFile, std::ios::in );
       
        if ( l_shaderFile.is_open() )
        {
            std::stringstream l_stream;
            std::string l_shaderCode;

            l_stream << l_shaderFile.rdbuf();

            l_shaderFile.close();
            l_shaderCode = l_stream.str();

            l_return = createShader ( a_shaderType, l_shaderCode );
        }

        return l_return;
    }

    //-----------------------------------------------------------------------//
    GLuint CDisplayPlugin::createShader ( GLenum a_shaderType,
                                          std::string& ar_shaderCode )
    {
        GLuint l_shaderHandle = glCreateShader( a_shaderType );

        LOG4CXX_INFO ( m_logger, "Created Shader Handle : " << l_shaderHandle );

        const char *lp_shaderCode = ar_shaderCode.c_str();

        glShaderSource( l_shaderHandle, 1, &lp_shaderCode, NULL);

        LOG4CXX_DEBUG ( m_logger, "Loaded Shader Source: \n" << ar_shaderCode );

        glCompileShader( l_shaderHandle );
    
        GLint l_status;
    
        glGetShaderiv( l_shaderHandle, GL_COMPILE_STATUS, &l_status);

        LOG4CXX_INFO ( m_logger, "Compiled shader. Checking Status " <<
                                 "[status = " << l_status << "] " <<
                                 "[GL_FALSE = " << (uint32_t)GL_FALSE << "] " );
    
        if ( l_status == GL_FALSE)
        {
            GLint l_infoLogLength;
            glGetShaderiv(l_shaderHandle, GL_INFO_LOG_LENGTH, &l_infoLogLength);
    
            GLchar *lp_strInfoLog = new GLchar[l_infoLogLength + 1];
    
            glGetShaderInfoLog(l_shaderHandle, l_infoLogLength, NULL, lp_strInfoLog);
    
            const char *lp_strShaderType = NULL;
    
            switch( a_shaderType )
            {
                case GL_VERTEX_SHADER: lp_strShaderType = "vertex"; break;
                case GL_GEOMETRY_SHADER: lp_strShaderType = "geometry"; break;
                case GL_FRAGMENT_SHADER: lp_strShaderType = "fragment"; break;
    
            }
    
            LOG4CXX_ERROR ( m_logger, "Compile failure in " <<
                                      std::string(lp_strShaderType) <<
                                      " shader: " <<
                                      lp_strInfoLog );

            delete[] lp_strInfoLog;
        }
        else
        {
            LOG4CXX_DEBUG ( m_logger, "Created Shader [" << l_shaderHandle << "]" << ": " << 
                                     ar_shaderCode );                        
        }

        return l_shaderHandle;
    }

    //-----------------------------------------------------------------------//
    GLuint CDisplayPlugin::createProgram ( CDisplayPlugin::tShaderVec& ar_shaderVector )
    {
        GLuint l_programHandle = glCreateProgram();
    
        LOG4CXX_INFO ( m_logger, "Attaching Shaders to Shader program" );
        for(size_t i = 0; i < ar_shaderVector.size(); ++i )
        {
            glAttachShader( l_programHandle, ar_shaderVector[i]);
        } 
    
        LOG4CXX_INFO ( m_logger, "Linking shader program" );
        glLinkProgram(l_programHandle);
    
        GLint l_status;
    
        glGetProgramiv (l_programHandle, GL_LINK_STATUS, &l_status);
    
        if (l_status == GL_FALSE)
        {
            GLint l_infoLogLength;
    
            glGetProgramiv( l_programHandle, GL_INFO_LOG_LENGTH, &l_infoLogLength);
    
            GLchar *lp_strInfoLog = new GLchar[l_infoLogLength + 1];
    
            glGetProgramInfoLog( l_programHandle, l_infoLogLength, NULL, lp_strInfoLog);
    
            LOG4CXX_ERROR ( m_logger, "Linker failure: " << std::string ( lp_strInfoLog ) );
    
            delete[] lp_strInfoLog;
        }
        else
        {
            LOG4CXX_INFO ( m_logger, "Shader program created: " << l_programHandle );
        }
    
        LOG4CXX_INFO ( m_logger, "Detaching Shaders from Shader program" );
        for(size_t i = 0; i < ar_shaderVector.size(); ++i )
        {
            glDetachShader( l_programHandle, ar_shaderVector[i]);
            glDeleteShader( ar_shaderVector[i] );
        } 

        return l_programHandle;
    }
};

//-----------------------------------------------------------------------//
extern "C"
{
    framework::IPluggable* createInstance()
    {
        return new framework::CDisplayPlugin;
    }
}
