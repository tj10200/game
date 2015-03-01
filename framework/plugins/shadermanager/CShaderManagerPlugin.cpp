#include "CShaderManagerPlugin.h"
#include "CEventManager.h"

#include "json/json.h"
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>

#include "CPluginLoader.h"
#include "CShaderDB.h"
#include "IRenderable.h"

namespace framework
{
    const uint32_t CShaderManagerPlugin::SHADER_MANAGER_PLUGIN_ID = 1420387173;

    //-----------------------------------------------------------------------//
    CShaderManagerPlugin::CShaderManagerPlugin()
    :   IPluggable("CShaderManagerPlugin", SHADER_MANAGER_PLUGIN_ID),
        m_enabledShaderProgram ( 0 ),
        mp_shaderDB ( NULL )
    {}

    //-----------------------------------------------------------------------//
    CShaderManagerPlugin::~CShaderManagerPlugin()
    { 
        delete mp_shaderDB;
    }

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
            if ( false == l_shaderManagerNode.isMember ( "shader_db" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate shader database file" );
                l_ret = false;
            }
            else
            {
                mp_shaderDB = new CShaderDB;

                if ( false == ( l_ret = mp_shaderDB->initialize ( l_shaderManagerNode["shader_db"].asString() ) ) )
                {
                    LOG4CXX_ERROR ( m_logger, "Failed to initialize shader DB" );
                }
            }
        }

        if ( true == l_ret )
        {
            CEventManager* lp_eventManager = CEventManager::sGetInstance();
              
            lp_eventManager->registerEvent ( REGISTER_SHADER_EVENT,
                                             this,
                                             sRegisterShaderCallback,
                                             mp_registerShaderCallbackEvent 
                                           );

            lp_eventManager->registerEvent ( DEREGISTER_SHADER_EVENT,
                                             this,
                                             sDeregisterShaderCallback,
                                             mp_deregisterShaderCallbackEvent 
                                           );
        
            lp_eventManager->registerEvent ( RENDER_SHADER_EVENT,
                                             this,
                                             sRenderShaders,
                                             mp_renderShadersCallbackEvent 
                                           );
        
            lp_eventManager->registerEvent ( CAPTURE_PROGRAM_HANDLE_EVENT,
                                             this,
                                             sCaptureShaderHandle,
                                             mp_captureProgramIdEvent
                                           );
        
            lp_eventManager->registerEvent ( CAPTURE_UNIFORM_HANDLE_EVENT,
                                             this,
                                             sCaptureUniformHandle,
                                             mp_captureUniformHandleEvent
                                           );
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
    void CShaderManagerPlugin::sRegisterShaderCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->registerShaderCallback ( (tShaderRegistrationEvent*)ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::registerShaderCallback ( tShaderRegistrationEvent* ap_data )
    {
        tShaderProgramMap::iterator l_iter = m_shaderPrograms.find ( ap_data->first );

        if ( m_shaderPrograms.end() == l_iter )
        {
            CShaderDB::SShaderEntry* lp_shader = NULL;
            SShaderData l_data; 

            if ( true == mp_shaderDB->getShaderEntry ( ap_data->first, lp_shader ) )
            {
                CShaderManagerPlugin::tShaderVec l_compiledShaders;

                for ( int i = 0; i < lp_shader->m_shaderFiles.size(); ++i )
                {
                    std::string l_file = lp_shader->m_shaderFiles[i];

                    if ( l_file != "" )
                    {
                        GLenum l_type = lp_shader->m_shaderTypes[i];

                        l_compiledShaders.insert ( loadShader ( l_type,
                                                                l_file) );
                    }
                }

                tShaderRenderEvents l_newEvent;
                l_newEvent.first = createProgram ( l_compiledShaders );
                l_newEvent.second.push_back ( ap_data->second );
            
                m_shaderPrograms.insert ( std::make_pair ( ap_data->first, l_newEvent ) );
            }
            else
            {
                LOG4CXX_ERROR ( m_logger, "Failed to locate shader entry in db ( " << ap_data->first << " ) " );
            }
        }
        else
        {
            //Shader already created. Just add IRenderable to list
            l_iter->second.second.push_back ( ap_data->second );
        }
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sDeregisterShaderCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CShaderManagerPlugin* >(ap_instance)->deregisterShaderCallback ( (tShaderRegistrationEvent*)ap_data );
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::deregisterShaderCallback ( tShaderRegistrationEvent* ap_data )
    {
        tShaderProgramMap::iterator l_iter = m_shaderPrograms.find ( ap_data->first );

        //If at end, then this shader is not registered.
        if ( m_shaderPrograms.end() != l_iter )
        {
            tShaderRenderEvents l_event = l_iter->second;
            std::vector < IRenderable* >::iterator l_renderIter = l_event.second.begin();

            //Find our renderable object and remove it
            while ( l_renderIter != l_event.second.begin() )
            {
                if ( (*l_renderIter) == ap_data->second )
                {
                    l_event.second.erase ( l_renderIter );
                    break;
                }

                ++l_renderIter;
            }

            if ( 0 == l_event.second.size() )
            {
                glDeleteProgram ( l_iter->second.first );
                m_shaderPrograms.erase ( l_iter );
            }
        }
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sRenderShaders( void* ap_instance, void* ap_data )
    {
        static_cast<CShaderManagerPlugin*>(ap_instance)->renderShaders ();
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::renderShaders()
    {
        tShaderProgramMap::iterator l_iter = m_shaderPrograms.begin();
        tShaderRenderEvents l_event;
        std::vector < IRenderable* >::iterator l_renderIter;
        IRenderable* lp_renderable = NULL;


        while ( l_iter != m_shaderPrograms.end() )
        {
            l_event = l_iter->second;
            l_renderIter = l_event.second.begin();
            lp_renderable = NULL;

            while ( l_renderIter != l_event.second.end() )
            {
                lp_renderable = (*l_renderIter++);

                lp_renderable->render();

            }

            ++l_iter;
        }
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sCaptureShaderHandle ( void* ap_instance, void* ap_data )
    {
        static_cast<CShaderManagerPlugin*>(ap_instance)->captureShaderHandle ( (SShaderProgramData*)ap_data);
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::captureShaderHandle ( SShaderProgramData* ap_data )
    {
        tShaderProgramMap::iterator l_iter = m_shaderPrograms.find ( ap_data->m_name );

        if ( l_iter != m_shaderPrograms.end() )
        {
            ap_data->m_programId = l_iter->second.first;
        }
        else
        {
            ap_data->m_programId = -1;
        }

    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::sCaptureUniformHandle ( void* ap_instance, void* ap_data )
    {
        static_cast<CShaderManagerPlugin*>(ap_instance)->captureUniformHandle( (SShaderUniformData*)ap_data);
    }

    //-----------------------------------------------------------------------//
    void CShaderManagerPlugin::captureUniformHandle ( SShaderUniformData* ap_data )
    {
        ap_data->m_uniformHandle = glGetUniformLocation ( ap_data->m_programId, 
                                                          ap_data->m_uniformName.c_str() );
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
