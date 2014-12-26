#ifndef CManager_h
#define CManager_h

#include "IPluggable.h"

#include <stdint.h>
#include "log4cxx/logger.h"
#include <boost/thread.hpp>

namespace framework
{
    class CEventManager;
    class CEvent;

    class CGLManager: public IPluggable
    {
        public:
            
            /** The global ID for this object **/
            static const uint32_t GL_MANAGER_ID;
            
            /**
             * Constructor
             */
            CGLManager();
            
            /** 
             * Destructor
             */
            virtual ~CGLManager();
            
            /**
             * Load config data from Json node
             *
             * @param ar_node - the JSON root node
             */
            virtual bool loadConfig ( const Json::Value& ar_node );

            /**
             * Starts the application
             */
            void start();

            /**
             * Stops the application
             */
            void stop();
        
            /**
             * Initializes the GLUT context
             */
            void initializeContext();

            /**
             * The local copy of the display function
             * Note that this object will just call into the registered version
             */
            static void  sDisplayFunction ();
            void displayFunction();

            /**
             * The local copy of the reshape function
             * Note that this object will just call into the registered version
             */
            static void  sReshapeFunction ( int a_width, int a_height );
            void reshapeFunction( int a_width, int a_height );

            /**
             * The local copy of the keyboard function
             * Note that this object will just call into the registered version
             */
            static void  sKeyboardFunction ( unsigned char a_key,
                                             int a_mouseX, 
                                             int a_mouseY );
            void keyboardFunction( unsigned char a_key,
                                   int a_mouseX,
                                   int a_mouseY );

            /**
             * The local copy of the window close function
             * Note that this object will just call into the registered version
             */
            static void  sCloseFunction ();
            void closeFunction();

            /**
             * The local copy of the idle function
             * Note that this object will just call into the registered version
             */
            static void  sIdleFunction ();
            void idleFunction();

            /**
             * The GLUT main loop thread
             */
            void threadFunc();

            /**
             * Gets the loaded instance of the manager object
             *
             * @return CGLManager* - the loaded object
             */
            static CGLManager* sGetLoadedInstance();

        private:

            /** Window Width **/
            int m_windowWidth;

            /** Window Height **/
            int m_windowHeight;
            
            /** Window Start Position **/
            int m_windowStartHorizontal;
            
            /** Window Start Position **/
            int m_windowStartVertical;

            /** Display Mode Flags **/
            uint32_t m_displayMode;

            /** The Event Manager Instance **/
            CEventManager* mp_eventManager;
            
            /** The Display Event **/
            CEvent* mp_displayEvent;

            /** The Reshape Event **/
            CEvent* mp_reshapeEvent;

            /** The Keyboard Event **/
            CEvent* mp_keyboardEvent;

            /** The Close Event **/
            CEvent* mp_windowCloseEvent;

            /** The Idle Event **/
            CEvent* mp_idleEvent;

            /** The GLUT Main Loop thread **/
            boost::thread m_thread;
    };

};

#endif
