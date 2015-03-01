#ifndef CShaderManagerPlugin_h
#define CShaderManagerPlugin_h

#include <string>
#include <unordered_map>

#include "IPluggable.h"
#include "ShaderManagerCommon.h"

#include "log4cxx/logger.h"
#include "GL/freeglut.h"
#include "TVector.h"

namespace framework
{
    class CEvent;
    class IRenderable;
    class CShaderDB;

    class CShaderManagerPlugin: public IPluggable
    {
        public:
            static const uint32_t SHADER_MANAGER_PLUGIN_ID;

            typedef std::pair < GLuint, std::string > tShaderFile;
            typedef containers::TVector < tShaderFile, uint8_t > tShaderFileVec;
            typedef containers::TVector < GLuint, uint16_t > tCompiledShaders;
            typedef containers::TVector< GLuint, uint16_t > tShaderVec;

            /** The mapping of shader program to things that need to be rendered **/
            typedef std::pair < GLuint, std::vector < IRenderable* > > tShaderRenderEvents;
            typedef std::unordered_map < std::string, tShaderRenderEvents > tShaderProgramMap;

            struct SShaderData 
            {
                /**
                 * internal tag identifier for the shader
                 */
                uint16_t m_shaderTag;
                
                /**
                 * Vector of shader files
                 */
                tShaderFileVec m_shaderFiles;
            };

            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            CShaderManagerPlugin();
    
            /**
             * Destructor
             */
            virtual ~CShaderManagerPlugin();
    
            /**
             * Loads configuration data
             * 
             * @param ar_node - the root node for this object
             * @return bool - true on successful config load
             */
            virtual bool loadConfig ( const Json::Value& ar_node );
    
            /**
             * Starts the object
             */
            virtual void start();
    
            /**
             * Stops the object
             */
            virtual void stop();

            /**
             * Register an IRenderable object against a shader program
             *
             * @param ap_instance - the shader manager instance
             * @param ap_data - a tShaderRegistrationEvent
             */
            static void sRegisterShaderCallback ( void* ap_instance,
                                                  void* ap_data );
            void registerShaderCallback ( tShaderRegistrationEvent* ap_data );
            
            /**
             * Deregister an IRenderable object from a shader program
             *
             * @param ap_instance - the shader manager instance
             * @param ap_data - a tShaderRegistrationEvent
             */
            static void sDeregisterShaderCallback ( void* ap_instance,
                                                    void* ap_data );
            void deregisterShaderCallback ( tShaderRegistrationEvent* ap_data );

            /**
             * Render shaders
             *
             * @param ap_instance - the shader manader instance
             * @param ap_data - NULL
             */
            static void sRenderShaders ( void* ap_instance, void* ap_data );
            void renderShaders();
            
            /**
             * Captures the program ID 
             *
             * @param ap_data - struct with shader name. returns shader program id
             */
            static void sCaptureShaderHandle ( void* ap_instance, void* ap_data );
            void captureShaderHandle ( SShaderProgramData* ap_data );

            /**
             * Captures a uniform handle for a single shader
             *
             * @param ap_data - struct with the program id and the uniform name.
             * function will populate the handle
             */
            static void sCaptureUniformHandle ( void* ap_instance, void* ap_data );
            void captureUniformHandle ( SShaderUniformData* ap_data );

        private:

            /**
             * Loads the shader code from a file,
             * and uses it to create the shader
             *
             * @param a_shaderType - the shader type
             * @param ar_shaderFile - the shader file
             * @return GLuint - the shader handle
             */
            GLuint loadShader ( GLenum a_shaderType,
                                std::string& ar_shaderFile );
            
            /**
             * Create Shader
             *
             * @param a_shaderType - the shader type
             * @param a_shaderCode - the shader function
             * @return GLuint - the shader handle
             */
            GLuint createShader ( GLenum a_shaderType,
                                  std::string& ar_shaderCode );

            
            /**
             * Creates a shader program
             *
             * @param ar_shaderVector& - the shader program vector
             * @return GLuint - the program handle
             */
            GLuint createProgram ( tShaderVec& ar_shaderVector );

        private:

            /** The Register Shader Callback Event object **/
            CEvent* mp_registerShaderCallbackEvent;

            /** The Deregister Shader Callback Event Object **/
            CEvent* mp_deregisterShaderCallbackEvent;

            /** The Render Shaders Callback Event Object **/
            CEvent* mp_renderShadersCallbackEvent;

            /** The Capture Program ID Callback Event Object **/
            CEvent* mp_captureProgramIdEvent;

            /** The Capture Uniform Handle Callback Event Object **/
            CEvent* mp_captureUniformHandleEvent;

            /** The currently enabled shader program **/
            GLuint m_enabledShaderProgram;

            /** The hash map of shader programs **/
            tShaderProgramMap m_shaderPrograms;

            /** The shader DB **/
            CShaderDB* mp_shaderDB;
    };
};
 
#endif
