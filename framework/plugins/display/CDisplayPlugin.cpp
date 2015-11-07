#include "CDisplayPlugin.h"
#include "CEventManager.h"
#include "CEvent.h"

#include "json/json.h"
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>

#include "GLManagerEvents.h"

#include "CPluginLoader.h"
#include "IRenderable.h"
#include "ShaderManagerCommon.h"

namespace framework
{
    const uint32_t DISPLAY_PLUGIN_ID = 1405198146;
    IPluggable * gp_renderable = NULL;

    //-----------------------------------------------------------------------//
    CDisplayPlugin::CDisplayPlugin()
    :   IRenderable("CDisplayPlugin", DISPLAY_PLUGIN_ID),
        m_windowSizeUniform(0),
        m_windowSize(0)
    {}

    //-----------------------------------------------------------------------//
    CDisplayPlugin::~CDisplayPlugin()
    {
        delete mp_renderEvent;
    }

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
        
        if ( true == l_ret )
        {
            mp_renderEvent = new CEvent ( RENDER_SHADER_EVENT );
        }

        if ( true == l_ret )
        {
            if ( false == l_displayNode.isMember ( "shader_name" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate shader node" );
                l_ret = false;
            }
            else
            {
                m_shaderName = l_displayNode["shader_name"].asString();
            }
        }


        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::start()
    {
        gp_renderable->start();

        tShaderRegistrationEvent l_shaderEvent;
        l_shaderEvent.first = m_shaderName;
        l_shaderEvent.second = this;

        CEvent l_registerShader ( REGISTER_SHADER_EVENT );
        CEvent l_captureProgram ( CAPTURE_PROGRAM_HANDLE_EVENT );
        CEvent l_captureUniform ( CAPTURE_UNIFORM_HANDLE_EVENT );

        //First load and compile the shader
        CEventManager::sGetInstance()->publishEvent ( &l_registerShader, (void*)(&l_shaderEvent) );

        //Get the program id from the shader manader
        SShaderProgramData l_programId;
        l_programId.m_name = m_shaderName;
        CEventManager::sGetInstance()->publishEvent ( &l_captureProgram, (void*)(&l_programId) );
        m_shaderProgramHandle = l_programId.m_programId;

        //Get the window size uniform from the shader manager
        SShaderUniformData l_uniformId;
        l_uniformId.m_programId = m_shaderProgramHandle;
        l_uniformId.m_uniformName = "windowSize";

        CEventManager::sGetInstance()->publishEvent ( &l_captureUniform, (void*)(&l_uniformId) );
        m_windowSizeUniform = l_uniformId.m_uniformHandle;

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
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        CEventManager::sGetInstance()->publishEvent ( mp_renderEvent, NULL );

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
    void CDisplayPlugin::render()
    {
        glUniform1f ( m_windowSizeUniform, m_windowSize );
        static_cast<IRenderable*>(gp_renderable)->render();
    }

    //-----------------------------------------------------------------------//
    void CDisplayPlugin::updateUniforms()
    {
        glUniform1f ( m_windowSizeUniform, m_windowSize );
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
