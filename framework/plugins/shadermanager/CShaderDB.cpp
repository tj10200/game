#include "CShaderDB.h"

#include "json/json.h"

#include <fstream>

namespace framework
{
    //-----------------------------------------------------------------------//
    CShaderDB::CShaderDB()
    : m_logger ( log4cxx::Logger::getLogger ( "CShaderDB" ) )
    {}

    //-----------------------------------------------------------------------//
    CShaderDB::~CShaderDB()
    {
        m_shaderDB.clear();
    }
    
    //-----------------------------------------------------------------------//
    bool CShaderDB::initialize ( std::string a_file )
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
            if ( true == l_rootNode["shaders"].isArray() )
            {
                m_shaderDB.reserve ( l_rootNode["shaders"].size() );

                for ( int i = 0; i < l_rootNode["shaders"].size() && l_ret; i++ )
                {
                    Json::Value l_shader= l_rootNode["shaders"][i];
                
                    SShaderEntry l_shaderEntry;

                    if ( true == l_shader.isMember( "shader_name" ) )
                    {
                        l_shaderEntry.m_shaderName = l_shader["shader_name"].asString();
                        l_shaderEntry.m_shaderFiles.reserve ( 6 );

                        LOG4CXX_INFO ( m_logger, "Loading shader: " <<
                                                 l_shader["shader_name"].asString() );
                        
                        /**
                         * Load the shader files in this order
                         *
                         * 1 ) Vertex shader - required
                         * 2 ) Fragment shader - required
                         * 3 ) Geometry shader - optional
                         * 4 ) tesselation shader - optional : requires OpenGL > 4.0
                         * 5 ) evaluation shader - optional : requires OpenGL > 4.0
                         * 6 ) compute shader - optional  : requires OpenGL > 4.3
                         */

                        //Vertex Shader
                        if ( true == l_shader.isMember ( "vertex_shader" ) )
                        {
                            l_shaderEntry.m_shaderTypes.push_back ( GL_VERTEX_SHADER );
                            l_shaderEntry.m_shaderFiles.push_back ( l_shader[ "vertex_shader" ].asString() );
                        }
                        else
                        {
                            LOG4CXX_ERROR ( m_logger, "Shader : " << l_shaderEntry.m_shaderName << "didn't contain a vertex shader" );
                            l_ret = false;
                            break;
                        }

                        //Fragment shader
                        if ( true == l_shader.isMember ( "fragment_shader" ) )
                        {
                            l_shaderEntry.m_shaderTypes.push_back ( GL_FRAGMENT_SHADER );
                            l_shaderEntry.m_shaderFiles.push_back ( l_shader[ "fragment_shader" ].asString() );
                        }
                        else
                        {
                            LOG4CXX_ERROR ( m_logger, "Shader : " << l_shaderEntry.m_shaderName << "didn't contain a fragment shader" );
                            l_ret = false;
                            break;
                        }

                        //Geometry shader
                        if ( true == l_shader.isMember ( "geometry_shader" ) )
                        {
                            l_shaderEntry.m_shaderTypes.push_back ( GL_GEOMETRY_SHADER );
                            l_shaderEntry.m_shaderFiles.push_back ( l_shader[ "geometry_shader" ].asString() );
                        }
                        else
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( "" );
                        }

                        //tessellation shader
                        if ( true == l_shader.isMember ( "tessellation_shader" ) )
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( l_shader[ "tessellation_shader" ].asString() );
                        }
                        else
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( "" );
                        }

                        //evaluation shader
                        if ( true == l_shader.isMember ( "evaluation_shader" ) )
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( l_shader[ "evaluation_shader" ].asString() );
                        }
                        else
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( "" );
                        }

                        //compute shader
                        if ( true == l_shader.isMember ( "compute_shader" ) )
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( l_shader[ "compute_shader" ].asString() );
                        }
                        else
                        {
                            l_shaderEntry.m_shaderFiles.push_back ( "" );
                        }

                        m_shaderDB.push_back ( l_shaderEntry );
                        m_shaderDBIndex [ l_shaderEntry.m_shaderName ] = m_shaderDB.size() - 1;

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

    //-----------------------------------------------------------------------//
    bool CShaderDB::getShaderEntry ( const std::string& ar_name, 
                                     SShaderEntry*& apr_entry )
    {
        bool l_ret = true;

        std::unordered_map < std::string, uint16_t >::iterator l_iter;

        l_iter = m_shaderDBIndex.find ( ar_name );

        if ( m_shaderDBIndex.end() == l_iter )
        {
            apr_entry = NULL;
            l_ret = false;
        }
        else
        {
            apr_entry = (&m_shaderDB [ l_iter->second ]);
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CShaderDB::getShaderEntry ( const uint16_t a_id,
                                     const SShaderEntry*& apr_entry )
    {
        bool l_ret = true;

        apr_entry = (&m_shaderDB [ a_id ]);

        return l_ret;
    }
    
};
