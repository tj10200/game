#ifndef IPluggable_h
#define IPluggable_h

#include "log4cxx/logger.h"

/** Forward declaration for config node **/
namespace Json
{
    class Value;
};

namespace framework
{
    class IPluggable
    {
        public:
            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            IPluggable( const char* ap_name );
    
            /**
             * Destructor
             */
            virtual ~IPluggable();
    
            /**
             * Loads configuration data
             * 
             * @param ar_node - the root node for this object
             * @return bool - true on successful config load
             */
            virtual bool loadConfig ( const Json::Value& ar_node ) =0;
    
            /**
             * Starts the object
             */
            virtual void start() =0;
    
            /**
             * Stops the object
             */
            virtual void stop() =0;

            /**
             * Gets the name of the object
             */
            const std::string& getName(); 

        protected:
    
            /** The Logger Pointer **/
            log4cxx::LoggerPtr m_logger;
    
            /** The name of the object root config node **/
            std::string m_rootNode;

            /** The name of the object **/
            std::string m_name;
    };
};
 
#endif
