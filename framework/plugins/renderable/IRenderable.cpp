#include "IRenderable.h"
#include "json/json.h"
#include <fstream>

namespace framework
{
    std::string IRenderable::ms_shaderDir = "";

    //-----------------------------------------------------------------------//
    IRenderable::IRenderable ( const char* ap_name,
                               const uint16_t a_id )
    :   IPluggable ( ap_name, a_id ),
        m_vertexBufferHandle ( 0 ),
        m_shaderProgramHandle ( 0 )
    {}

    //-----------------------------------------------------------------------//
    IRenderable::~IRenderable()
    {
    }

    //-----------------------------------------------------------------------//
    void IRenderable::computeHash()
    {
        setHashCode ( m_id );
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::equals ( const THashable* const & apr_two ) const
    {
        bool l_ret = true;

        IRenderable* lp_two = (IRenderable*)apr_two;

        l_ret = ( m_id == lp_two->m_id );

        return l_ret;

    }

    //-----------------------------------------------------------------------//
    void IRenderable::update( const float& ar_time )
    {
       m_preChildren.visit ( IRenderable::sVisitorUpdate, ar_time ); 
       m_postChildren.visit ( IRenderable::sVisitorUpdate, ar_time ); 
    }

    //-----------------------------------------------------------------------//
    void IRenderable::addPreChild ( IRenderable* ap_child )
    {
        m_preChildren.insert ( ap_child );
    }

    //-----------------------------------------------------------------------//
    IRenderable* IRenderable::getPreChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::removePreChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    void IRenderable::addPostChild ( IRenderable* ap_child )
    {
    }

    //-----------------------------------------------------------------------//
    IRenderable* IRenderable::getPostChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::removePostChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    void IRenderable::sVisitorUpdate ( IRenderable* ap_obj, const float& ar_data )
    {
        ap_obj->update ( ar_data );
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::loadShaders ( Json::Value& ar_rootNode )
    {
        bool l_ret = true;

        //Now load the shader programs
        tShaderVec l_shaderVec;

        if ( true == l_ret )
        {
            if ( false == ar_rootNode.isMember ( "vertex_shader" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display vertex shader node" );
                l_ret = false;
            }
            else
            {
                std::string l_vertexShader = ar_rootNode["vertex_shader"].asString();
                l_shaderVec.push_back ( loadShader( GL_VERTEX_SHADER,
                                                    l_vertexShader ) );
            }
        }

        if ( true == l_ret )
        {
            if ( false == ar_rootNode.isMember ( "frag_shader" ) )
            {
                LOG4CXX_ERROR ( m_logger, "Unable to locate display fragment shader node" );
                l_ret = false;
            }
            else
            {
                std::string l_fragShader = ar_rootNode["frag_shader"].asString();
                l_shaderVec.push_back ( loadShader( GL_FRAGMENT_SHADER,
                                                    l_fragShader ) );
            }
        }
        
    }

    //-----------------------------------------------------------------------//
    GLuint IRenderable::loadShader ( GLenum a_shaderType,
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
    GLuint IRenderable::createShader ( GLenum a_shaderType,
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
    GLuint IRenderable::createProgram ( IRenderable::tShaderVec& ar_shaderVector )
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
}
