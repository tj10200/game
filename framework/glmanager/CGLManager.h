#ifndef CManager_h
#define CManager_h

#include "IPluggable.h"

#include <stdint.h>
#include "log4cxx/logger.h"

namespace framework
{
    class CGLManager: public IPluggable
    {
        public:
            
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
    };

};

#endif
