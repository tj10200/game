#ifndef IPluggable_h
#define IPluggable_h

#include "log4cxx/logger.h"
#include <stdint.h>
#include <functional>

/** Forward declaration for config node **/
namespace Json
{
    class Value;
};

namespace framework
{
    class IPluggable
    {
        public: //hash object

            struct SPluggableHash
            {
                size_t operator() ( const IPluggable* const & ar_key ) const
                {
                    std::hash<uint32_t> l_hasher;
                    return l_hasher ( ar_key->getId() );
                }
            };

            struct SPluggableEqual
            {
                bool operator() ( const IPluggable* const & ar_one,
                                  const IPluggable* const & ar_two ) const
                {
                    bool l_ret = false;

                    if ( ar_one != ar_two )
                    {
                        l_ret = ar_one->getId() == ar_two->getId() &&
                                ar_one->getInstanceId() == ar_two->getInstanceId();
                    }
                    else
                    {
                        l_ret = true;
                    }

                    return l_ret;
                }
            };

        public:
            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             * @param a_id - the id of this object
             */
            IPluggable( const char* ap_name,
                        const uint32_t a_id  );
    
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
             * Updates the plugin with the current time
             *
             * @param ar_elapsedSeconds - the seconds elapsed since program start
             */
            virtual void update ( const float& ar_elapsedSeconds );

            /**
             * Gets the name of the object
             */
            const std::string& getName(); 

            /**
             * Gets the ID of the object
             */
            uint32_t getId() const;

            /**
             * Sets the instance ID
             *
             * @param a_instance - the id to set
             */
            void setInstanceId( uint32_t a_instance );

            /**
             * Gets the instance ID
             *
             * @return uint32_t - instance ID
             */
            uint32_t getInstanceId() const;

        protected:
    
            /** The Logger Pointer **/
            log4cxx::LoggerPtr m_logger;
    
            /** The name of the object root config node **/
            std::string m_rootNode;

            /** The name of the object **/
            const std::string m_name;
            
            /** The plugin ID **/
            uint32_t m_id;
            
            /** The instance ID **/
            uint32_t m_instanceId;
    };
};
 
#endif
