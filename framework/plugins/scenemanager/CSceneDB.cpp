#include "CSceneDB.h"

#include "json/json.h"

#include <fstream>

namespace framework
{
    //-----------------------------------------------------------------------//
    CSceneDB::CSceneDB()
    : m_logger ( log4cxx::Logger::getLogger ( "CSceneDB" ) )
    {}

    //-----------------------------------------------------------------------//
    CSceneDB::~CSceneDB()
    {
        m_sceneDB.clear();
    }
    
    //-----------------------------------------------------------------------//
    bool CSceneDB::initialize ( std::string a_file )
    {
        bool l_ret = true;

        Json::Value l_rootNode; 
        Json::Reader l_reader;
        std::ifstream l_testFile ( a_file.c_str(), std::ifstream::binary);

        if ( false == l_reader.parse( l_testFile, l_rootNode ) )
        {
            LOG4CXX_ERROR ( m_logger, "Unable to parse Json file" );
           
            l_ret = false; 
        }

        if ( true == l_ret )
        {
            if ( true == l_rootNode["scenes"].isArray() )
            {
                m_sceneDB.reserve ( l_rootNode["scenes"].size() );

                for ( int i = 0; i < l_rootNode["scenes"].size() && l_ret; i++ )
                {
                    Json::Value l_scene= l_rootNode["scenes"][i];
                
                    SScene l_sceneEntry;

                    if ( true == l_scene.isMember( "scene_name" ) )
                    {
                        l_sceneEntry.m_sceneName = l_scene["scene_name"].asString();

                        LOG4CXX_INFO ( m_logger, "Loading scene: " <<
                                                 l_scene["scene_name"].asString() );
                        
                        //Loop through the renderables and store them in the vector
                        if ( true == l_scene.isMember ( "renderables" ) && 
                             true == l_scene[ "renderables" ].isArray() )
                        {
                            for ( int x = 0; x < l_scene["renderables"].size() && l_ret; ++x )
                            {
                                Json::Value l_renderable = l_scene["renderables"][x];

                                l_sceneEntry.m_renderableLibs.push_back ( l_renderable.asString() );
                            }

                            if ( true == l_scene.isMember( "shader_program" ) )
                            {
                                Json::Value l_shader = l_scene[ "shader_program" ];
                                
                                if ( true == l_shader.isMember ( "shader_num" ) )
                                {
                                    l_sceneEntry.m_shaderEntry.first = l_shader[ "shader_num" ].asInt();
                                }
                                else
                                {
                                    LOG4CXX_ERROR ( m_logger, "Scene: " << l_sceneEntry.m_sceneName << "didn't contain a shader program number" );
                                }

                                if ( true == l_shader.isMember ( "shader_name" ) )
                                {
                                    l_sceneEntry.m_shaderEntry.second = l_shader[ "shader_name" ].asString();
                                }
                                else
                                {
                                    LOG4CXX_ERROR ( m_logger, "Scene: " << l_sceneEntry.m_sceneName << "didn't contain a shader program name" );
                                }
                            }
                            else
                            {
                                LOG4CXX_ERROR ( m_logger, "Scene: " << l_sceneEntry.m_sceneName << "didn't contain a shader program" );
                            }
                                 
                        }
                        else
                        {
                            LOG4CXX_ERROR ( m_logger, "Scene: " << l_sceneEntry.m_sceneName << "didn't contain a renderables array" );
                            l_ret = false;
                            break;
                        }

                        m_sceneDB.push_back ( l_sceneEntry );
                        m_sceneDBIndex [ l_sceneEntry.m_sceneName ] = m_sceneDB.size() - 1;
                    }
                }
            }
            else
            {
                LOG4CXX_ERROR ( m_logger, "ShaderDB root node not present in file : " << a_file );
                l_ret = false;
            }
        }

        return l_ret;
        
    }
    //
    //-----------------------------------------------------------------------//
    bool CSceneDB::getScene ( const uint16_t a_id,
                              SScene*& apr_scene )
    {
        bool l_ret = true;

        apr_scene = (&m_sceneDB [ a_id ]);

        return l_ret;
    }
    
    //-----------------------------------------------------------------------//
    bool CSceneDB::getScene ( const std::string ar_name, 
                              SScene*& apr_scene )
    {
        bool l_ret = true;

        std::unordered_map < std::string, uint16_t >::iterator l_iter;

        l_iter = m_sceneDBIndex.find ( ar_name );

        if ( m_sceneDBIndex.end() == l_iter )
        {
            apr_scene = NULL;
            l_ret = false;
        }
        else
        {
            apr_scene = (&m_sceneDB [ l_iter->second ]);
        }

        return l_ret;
    }

};
