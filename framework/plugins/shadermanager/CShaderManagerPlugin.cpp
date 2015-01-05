#include "CShaderManagerPlugin.h"
#include "CEventManager.h"

#include "json/json.h"
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>

#include "CPluginLoader.h"

namespace framework
{
    const uint32_t SHADER_MANAGER_PLUGIN_ID = 1420387173;

    //-----------------------------------------------------------------------//
    CShaderManagerPlugin::CShaderManagerPlugin()
    :   IPluggable("CShaderManagerPlugin", SHADER_MANAGER_PLUGIN_ID),
        m_enabledShaderProgram ( 0 ),
        m_shaderPrograms ( 10, 10 )
    {}

    //-----------------------------------------------------------------------//
    CShaderManagerPlugin::~CShaderManagerPlugin()
    {}

    //-----------------------------------------------------------------------//
    bool CShaderManagerPlugin::loadConfig ( const Json::Value& ar_node )
    {
        bool l_ret = true;

        LOG4CXX_INFO ( m_logger, "Load Config" );
        
        Json::Value l_shaderManagerNode;

        if ( false == ar_node["shader_manager"].isObject() )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to locate the manager element" );
            l_ret = false;
        }
        else
        {
            l_shaderManagerNode = ar_node["shader_manager"]; 
        }

        if ( true == l_ret )
        {
            if ( false == l_shaderManagerNode.isMember ( "create_shader_event" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate create event subscriber name" );
                l_ret = false;
            }
            else
            {
                CEventManager* lp_eventManager = CEventManager::sGetInstance();
                
                lp_eventManager->registerEvent ( l_shaderManagerNode["create_shader_event"].asString().c_str(),
                                                 this,
                                                 sCreateShaderCallback,
                                                 mp_createShaderCallbackEvent 
                                               );
            }
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::start()
    {
    }
    
    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::stop()
    {
        LOG4CXX_INFO ( m_logger, "Stop" );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sCreateShaderCallback ( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->createShaderCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::createShaderCallback ( void* ap_data )
    {
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sDestroyShaderCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->destroyShaderCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::destroyShaderCallback( void* ap_data )
    {
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sDestroyProgramCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->destroyProgramCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::destroyProgramCallback( void* ap_data )
    {
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sEnableShaderCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->enableShaderCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::enableShaderCallback ( void* ap_data )
    {
        uint16_t l_shaderId = (uint16_t)*((uint16_t*)ap_data);

        GLuint l_shaderHandle = 0;

        if ( true == m_shaderPrograms.find ( l_shaderId, l_shaderHandle ) )
        {
            if ( 0 == m_enabledShaderProgram ||
                 m_enabledShaderProgram != l_shaderHandle )
            {
                m_enabledShaderProgram = l_shaderHandle;
                glUseProgram( l_shaderHandle );
            }
        }
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sDisableShaderCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->disableShaderCallback ( ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::disableShaderCallback ( void* ap_data )
    {
        m_enabledShaderProgram = 0;
        glUseProgram(0);
    }

    //-----------------------------------------------------------------------//
    GLuint CShaderManagerPlugin::loadShader ( GLenum a_shaderType,
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
    GLuint CShaderManagerPlugin::createShader ( GLenum a_shaderType,
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
    GLuint CShaderManagerPlugin::createProgram ( CShaderManagerPlugin::tShaderVec& ar_shaderVector )
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
        return new framework::CShaderManagerPlugin;
    }
}
