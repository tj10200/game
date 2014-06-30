#ifndef CPluggableStub_h
#define CPluggableStub_h

#include "IPluggable.h"
#include "log4cxx/logger.h"

namespace framework
{
    class CPluggableStub: public IPluggable
    {
        public:
            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            CPluggableStub();
    
            /**
             * Destructor
             */
            virtual ~CPluggableStub();
    
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
    };
};
 
#endif
