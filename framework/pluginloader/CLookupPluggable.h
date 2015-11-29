#ifndef CLookupPluggable_h
#define CLookupPluggable_h 

#include "IPluggable.h"

namespace framework
{
    class CLookupPluggable: public IPluggable 
    {
        public:
            /**
             * Constructor
             */
            CLookupPluggable ()
            : IPluggable ( "lookupplugin", 0 )
            {}
    
            /**
             * Destructor
             */
            virtual ~CLookupPluggable()
            {}
    
            /**
             * Loads configuration data
             * 
             * @param ar_node - the root node for this object
             * @return bool - true on successful config load
             */
            virtual bool loadConfig ( const Json::Value& ar_node )
            {
                return true;
            }
    
            /**
             * Starts the object
             */
            virtual void start()
            {}
    
            /**
             * Stops the object
             */
            virtual void stop()
            {}

            /**
             * Updates the plugin with the current time
             *
             * @param ar_elapsedSeconds - the seconds elapsed since program start
             */
            virtual void update ( const float& ar_elapsedSeconds )
            {}

            /**
             * Setter for the ID
             *
             * @param a_id - the id to set
             * */
            void setId( uint32_t a_id )
            {
                m_id = a_id;
            }

    };
};
 
#endif
