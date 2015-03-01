#ifndef ShaderManagerCommon_h
#define ShaderManagerCommon_h

#include <map>
#include <string>
#include "GL/freeglut.h"

namespace framework
{
    class IRenderable;

    static const char* const REGISTER_SHADER_EVENT          = "register_shader";
    static const char* const DEREGISTER_SHADER_EVENT        = "deregister_shader";
    static const char* const RENDER_SHADER_EVENT            = "render_shader";
    static const char* const CAPTURE_PROGRAM_HANDLE_EVENT   = "capture_program_handle";
    static const char* const CAPTURE_UNIFORM_HANDLE_EVENT   = "capture_uniform_handle";

    //Typedef used to register a shader name against a renderable object
    typedef std::pair < std::string, IRenderable* > tShaderRegistrationEvent;

    /** Struct used to capture a program Id given a named shader object **/
    struct SShaderProgramData
    {
        /**
         * The name of the shader
         */
        std::string m_name;

        /**
         * The shader program id
         */
        GLuint m_programId;

    };
    
    /** Struct used to capture a uniform id given a program id **/
    struct SShaderUniformData
    {
        /**
         * The shader program id
         */
        GLuint m_programId;

        /**
         * The shader uniform name
         */
        std::string m_uniformName;

        /**
         * The shader uniform handle
         */
        GLuint m_uniformHandle;
    };

};

#endif
