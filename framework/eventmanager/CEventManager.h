#ifndef CEventManager_h
#define CEventManager_h

#include "CSubscriberMap.h"

namespace framework
{
    class CEventManager
    {
        public:
            
            /**
             * Gets/Creates an instance of the object 
             */
            static CEventManager* sGetInstance();

            /**
             * Registers a new event 
             *
             * @param ap_event - the named event
             * @param ap_instance - the object instance
             * @param a_callback - the callback to hit
             * @param apr_event - the created event object
             * @return bool - true if successful
             */
            bool registerEvent ( const char* ap_event,
                                 void* ap_instance,
                                 tSubscriberCallback a_callback,
                                 CEvent*& apr_event );

            
            /**
             * Unregisteres an event
             *
             * @param ap_event - the event to remove
             * @param ap_instance - the object instance to remove
             * @return bool - true if found and removed
             */
            bool unregisterEvent ( CEvent* ap_event,
                                   void* ap_instance );

            
            /**
             * Publish data to an event
             *
             * @param ap_event - the event to publish
             * @param ap_data - the event data to publish
             * @param bool - true if there are subscribers
             */
            bool publishEvent ( CEvent* ap_event,
                                void* ap_data );

        
        private:
            /**
             * Constructor
             */
            CEventManager()
            {}

            /**
             * Destructor
             */
            virtual ~CEventManager()
            {}

            /**
             * Copy Constructor
             */
            CEventManager( const CEventManager& ar_other )
            {} 

       private:

            /** The map of events to subscribers **/
            CSubscriberMap m_subscriberMap;

    };
};

#endif
