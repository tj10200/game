#ifndef CDisplayPlugin_h
#define CDisplayPlugin_h

#include "IRenderable.h"
#include "log4cxx/logger.h"
#include "GL/freeglut.h"
#include <string>

namespace framework
{
    class CEvent;

    class CDisplayPlugin: public IRenderable
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

            /**
             * @inherited from base
             */
            virtual void render();

            /**
             * @inherited from base
             */
            virtual void updateUniforms();

        private:

            /** The Display Callback Event object **/
            CEvent* mp_displayCallbackEvent;
            
            /** The Reshape Callback Event object **/
            CEvent* mp_reshapeCallbackEvent;

            /** The Render Callback Event object for the shader manager **/
            CEvent* mp_renderEvent;

            /** The Shader Program **/
            GLuint m_shaderProgramHandle;
             
            /** The shader uniform window size **/
            GLuint m_windowSizeUniform;

            /** The window size **/
            uint16_t m_windowSize;

            /** The shader name **/
            std::string m_shaderName;
    };
};
 
#endif
