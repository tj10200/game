#ifndef CDisplayPlugin_h
#define CDisplayPlugin_h

#include "IPluggable.h"
#include "log4cxx/logger.h"
#include "GL/freeglut.h"
#include <string>

namespace framework
{
    class CEvent;

    class CDisplayPlugin: public IPluggable
    {
        public:

            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            CDisplayPlugin();
    
            /**
             * Destructor
             */
            virtual ~CDisplayPlugin();
    
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
             * The display callback function
             */
            static void sDisplayCallback ( void* ap_instance, 
                                           void* ap_data );
            void displayCallback ( void* ap_data );

            /**
             * The reshape callback function
             */
            static void sReshapeCallback ( void* ap_instance, 
                                           void* ap_data );
            void reshapeCallback ( void* ap_data );

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

            /** The Display Callback Event object **/
            CEvent* mp_displayCallbackEvent;
            
            /** The Reshape Callback Event object **/
            CEvent* mp_reshapeCallbackEvent;

            /** The Shader Program **/
            GLuint m_shaderProgramHandle;
             
            /** The shader uniform window size **/
            GLuint m_windowSizeUniform;

            /** The window size **/
            uint16_t m_windowSize;
    };
};
 
#endif
