#include "CSceneManagerPlugin.h"
#include "CEventManager.h"

#include "json/json.h"
#include <GL/freeglut.h>
#include <fstream>
#include <sstream>

#include "CPluginLoader.h"
#include "CSceneDB.h"
#include "CEventManager.h"
#include "CEvent.h"

#include "SceneManagerCommon.h"

#include "CPluginLoader.h"
#include "IRenderable.h"
#include "ShaderManagerCommon.h"

namespace framework
{
    const uint32_t CSceneManagerPlugin::SCENE_MANAGER_PLUGIN_ID = 1447725835;

    //-----------------------------------------------------------------------//
    CSceneManagerPlugin::CSceneManagerPlugin()
    :   IPluggable("CSceneManagerPlugin", SCENE_MANAGER_PLUGIN_ID),
        mp_sceneDB ( NULL )
    {}

    //-----------------------------------------------------------------------//
    CSceneManagerPlugin::~CSceneManagerPlugin()
    { 
        delete mp_sceneDB;
    }

    //-----------------------------------------------------------------------//
    bool CSceneManagerPlugin::loadConfig ( const Json::Value& ar_node )
    {
        bool l_ret = true;

        LOG4CXX_INFO ( m_logger, "Load Config" );
        
        Json::Value l_sceneManagerNode;

        if ( false == ar_node["scene_manager"].isObject() )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to locate the manager element" );
            l_ret = false;
        }
        else
        {
            l_sceneManagerNode = ar_node["scene_manager"]; 
        }

        if ( true == l_ret )
        {
            if ( false == l_sceneManagerNode.isMember ( "scene_db" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate scene database file" );
                l_ret = false;
            }
            else
            {
                mp_sceneDB = new CSceneDB;

                if ( false == ( l_ret = mp_sceneDB->initialize ( l_sceneManagerNode["scene_db"].asString() ) ) )
                {
                    LOG4CXX_ERROR ( m_logger, "Failed to initialize scene DB" );
                }
            }
        }

        if ( true == l_ret )
        {
            CEventManager* lp_eventManager = CEventManager::sGetInstance();
              
            lp_eventManager->registerEvent ( REGISTER_SCENE_EVENT,
                                             this,
                                             sRegisterSceneCallback,
                                             mp_loadSceneCallbackEvent 
                                           );

            lp_eventManager->registerEvent ( DEREGISTER_SCENE_EVENT,
                                             this,
                                             sDeregisterSceneCallback,
                                             mp_unloadSceneCallbackEvent 
                                           );
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CSceneManagerPlugin::start()
    {
    }
    
    //-----------------------------------------------------------------------//
    void CSceneManagerPlugin::stop()
    {
        LOG4CXX_INFO ( m_logger, "Stop" );
    }

    //-----------------------------------------------------------------------//
    void CSceneManagerPlugin::sRegisterSceneCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CSceneManagerPlugin* >(ap_instance)->registerSceneCallback ( (uint16_t*)ap_data );
    }

    //-----------------------------------------------------------------------//
    void CSceneManagerPlugin::registerSceneCallback ( uint16_t* ap_data )
    {
        CSceneDB::SScene* lp_scene = NULL;

        const uint16_t l_sceneNum = (*ap_data);

        if ( true == mp_sceneDB->getScene ( l_sceneNum, lp_scene ) )
        {
            framework::CPluginLoader* lp_loader = CPluginLoader::getInstance();

            tShaderRegistrationEvent l_shaderEvent;
            l_shaderEvent.first = lp_scene->m_shaderEntry.second;

            CEvent l_registerShader ( REGISTER_SHADER_EVENT );

            std::vector< std::string >::iterator l_iter = lp_scene->m_renderableLibs.begin();

            IPluggable* lp_plugin = NULL;
            while ( l_iter != lp_scene->m_renderableLibs.end() )
            {
                lp_loader->createPlugin ( (*l_iter), 
                                          lp_plugin );
                lp_plugin->start();

                LOG4CXX_INFO ( m_logger, "Created renderable : ID - " << lp_plugin->getId() << ", Instance - " << lp_plugin->getInstanceId() );

                lp_scene->m_renderables.push_back ( lp_plugin );

                //Register the display plugin renderable
                l_shaderEvent.second = static_cast<IRenderable*>(lp_plugin);

                CEventManager::sGetInstance()->publishEvent ( &l_registerShader, (void*)(&l_shaderEvent) );

                ++l_iter;
            }

        }
    }

    //-----------------------------------------------------------------------//
    void CSceneManagerPlugin::sDeregisterSceneCallback( void* ap_instance, void* ap_data )
    {
        static_cast< CSceneManagerPlugin* >(ap_instance)->deregisterSceneCallback ( (uint16_t*)ap_data );
    }

    //-----------------------------------------------------------------------//
    void CSceneManagerPlugin::deregisterSceneCallback ( uint16_t* ap_data )
    {
        CSceneDB::SScene* lp_scene = NULL;

        const uint16_t l_sceneNum = (*ap_data);

        if ( true == mp_sceneDB->getScene ( l_sceneNum, lp_scene ) )
        {
            framework::CPluginLoader* lp_loader = CPluginLoader::getInstance();

            lp_scene->m_renderables.clear();

            for ( int i = 0; i < lp_scene->m_renderableLibs.size(); ++i )
            {
                lp_loader->destroyPlugins ( lp_scene->m_renderableLibs[i] );
            }

            CEvent l_deregisterShader ( DEREGISTER_SHADER_EVENT );

            tShaderRegistrationEvent l_shaderEvent;
            l_shaderEvent.first = lp_scene->m_shaderEntry.second;
        
            CEventManager::sGetInstance()->publishEvent ( &l_deregisterShader, (void*)(&l_shaderEvent) );
        }
    }
};

//-----------------------------------------------------------------------//
extern "C"
{
    framework::IPluggable* createInstance()
    {
        return new framework::CSceneManagerPlugin;
    }
}
