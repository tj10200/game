#ifndef CRenderableManagerPlugin_h
#define CRenderableManagerPlugin_h

#include "IPluggable.h"
#include "GL/freeglut.h"
#include <unordered_map>

namespace framework
{
    class CEvent;

    class CRenderableManagerPlugin: public IPluggable
    {
        public:

            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            CRenderableManagerPlugin();
    
            /**
             * Destructor
             */
            virtual ~CRenderableManagerPlugin();
    
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
        
        private:
            
            typedef std::unordered_map< s, tRenderableInstanceFunc >;
    };
};
 
#endif
