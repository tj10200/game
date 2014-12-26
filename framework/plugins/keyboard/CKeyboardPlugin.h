#ifndef CKeyboardPlugin_h
#define CKeyboardPlugin_h

#include "IPluggable.h"
#include "log4cxx/logger.h"
#include "GL/freeglut.h"
#include <string>

namespace framework
{
    class CEvent;

    class CKeyboardPlugin: public IPluggable
    {
        public:
            typedef std::vector< GLuint > tShaderVec;

            /** Static ID **/
            static const uint32_t PLUGIN_ID;


            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            CKeyboardPlugin();
    
            /**
             * Destructor
             */
            virtual ~CKeyboardPlugin();
    
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
            static void sKeyboardCallback ( void* ap_instance, 
                                            void* ap_data );
            void keyboardCallback ( void* ap_data );

            /**
             * The reshape callback function
             */
            static void sReshapeCallback ( void* ap_instance, 
                                           void* ap_data );
            void reshapeCallback ( void* ap_data );

            /**
             * The window close callback function
             */
            static void sCloseCallback ( void* ap_instance, 
                                         void* ap_data );
            void closeCallback ( void* ap_data );

            /**
             * Getter for the leave main loop flag
             */
            bool getLeaveMainLoopFlag();

        private:

            /** The Keyboard Callback Event object **/
            CEvent* mp_keyboardCallbackEvent;
            
            /** The Window close Callback Event object **/
            CEvent* mp_closeCallbackEvent;
            
            /** The exit key was found **/
            bool m_leaveMainLoop;
    };
};
 
#endif
