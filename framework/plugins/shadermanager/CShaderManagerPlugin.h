#ifndef CShaderManagerPlugin_h
#define CShaderManagerPlugin_h

#include "IPluggable.h"
#include "log4cxx/logger.h"
#include "GL/freeglut.h"
#include <string>
#include "TVector.h"


namespace framework
{
    class CEvent;

    class CShaderManagerPlugin: public IPluggable
    {
        public:
            static const uint32_t SHADER_MANAGER_PLUGIN_ID;

            typedef std::pair < GLuint, std::string > tShaderFile;
            typedef containers::TVector < tShaderFile, uint8_t > tShaderFileVec;
            typedef containers::TVector < GLuint, uint16_t > tCompiledShaders;
            typedef containers::TVector< GLuint, uint16_t > tShaderVec;
            
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
             * Create shader callback function used to create a new shader
             * program
             *
             * @param ap_data - expects a std::vector of pairs of shader files to link into
             *  a shader program
             */
            static void sCreateShaderCallback ( void* ap_instance,
                                                void* ap_data );
            void createShaderCallback ( SShaderData* ap_data );
            
            /**
             * Destroy shader program callback function. Destroys a shader program
             *
             * @param ap_data - expects an id of the shader being destroyed
             */
            static void sDestroyProgramCallback ( void* ap_instance,
                                                  void* ap_data );
            void destroyProgramCallback ( uint16_t* ap_data );
            
            /**
             * The enable shader callback function
             */
            static void sEnableShaderCallback ( void* ap_instance, 
                                                void* ap_data );
            void enableShaderCallback ( uint16_t* ap_data );

            /**
             * The disable shader callback function
             */
            static void sDisableShaderCallback ( void* ap_instance, 
                                                 void* ap_data );
            void disableShaderCallback ( uint16_t* ap_data );

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

            /** The Create Shader Callback Event object **/
            CEvent* mp_createShaderCallbackEvent;
            
            /** The Destroy Shader Callback Event object **/
            CEvent* mp_destroyShaderCallbackEvent;
            
            /** The Destroy Program Callback Event object **/
            CEvent* mp_destroyProgramCallbackEvent;
            
            /** The Enable shader Callback Event object **/
            CEvent* mp_enableShaderCallbackEvent;

            /** The Disable shader Callback Event object **/
            CEvent* mp_disableShaderCallbackEvent;
            
            /** The currently enabled shader program **/
            GLuint m_enabledShaderProgram;

            /** The Shader Program **/
            containers::TVector <GLuint, uint16_t > m_shaderPrograms;
             
            /** The window size **/
            uint16_t m_windowSize;
    };
};
 
#endif
