#ifndef CSubscriberMap_h
#define CSubscriberMap_h

#include <list>
#include <stdint.h>
#include <unordered_map>
#include "THashable.h"

namespace framework
{
    class CEvent;

    /**
     * Callback used for event callbacks
     *
     * @param ap_instance - the object instance to callback
     * @param ap_event - the event used for publishing
     */
    typedef void ( *tSubscriberCallback ) ( void* ap_instance,
                                            void* ap_event );

    /**
     * Linked list used to store event subscribers
     */
    class CSubscriberList
    {
        public:

            /** The Stored List Element type **/
            typedef std::pair< uint64_t, tSubscriberCallback > tSubscriberListElem;

            /** The List **/
            typedef std::list< tSubscriberListElem > tSubscriberList;

            /** The List Iterator Type **/
            typedef tSubscriberList::iterator tSubscriberListIter;
            
    
            tSubscriberListIter begin()
            {
                return m_list.begin();
            }
    
            tSubscriberListIter end()
            {
                return m_list.end();
            }
    
            void erase ( tSubscriberListIter a_iter )
            {
                m_list.erase ( a_iter );
            }
    
            void insert ( tSubscriberListElem a_elem )
            {
                m_list.push_back ( a_elem );
            }
        

        private:

            tSubscriberList m_list; 
    };

    class CSubscriberMap
    {
        public:
            /** Map Typedefs **/
            typedef std::unordered_map< THashable<uint32_t>*, 
                                        CSubscriberList,
                                        THashable<uint32_t>::SHash,
                                        THashable<uint32_t>::SEquals> tEventMap;
            typedef tEventMap::iterator tEventMapIter;
            typedef std::list<uint64_t>::iterator tSubscriberListIter;

            /**
             * Constructor
             */
            CSubscriberMap();

            /**
             * Destructor
             */
            virtual ~CSubscriberMap();

            /**
             * Inserts a named event into the map
             *
             * @param ap_event - the named event string
             * @param ap_instance - the callback instance
             * @param a_callback - the subscriber callback
             * @param apr_event - the event to publish through
             * @return bool - true if inserted into map
             */
            bool insertEvent ( const char*   ap_event, 
                               void*         ap_instance,
                               tSubscriberCallback a_callback,
                               CEvent*&      apr_event );

            /**
             * Removes a named event from the map
             *
             * @param ap_event - the event to remove
             * @param ap_instance - the event to remove
             * @return bool - true if found and removed
             */
            bool removeEvent ( CEvent*      ap_event, 
                               void*        ap_instance );

            /**
             * Finds the list of subscribers associated with an event
             *
             * @param ap_event - the event to find
             * @param apr_list - the linked list of subscribers associated with the event
             * @return bool - true if found and returned
             */
            bool findEvent ( CEvent* ap_event, CSubscriberList*& apr_list );

        private:
            
            /** The event map **/
            tEventMap m_eventMap;
    
    };
};

#endif
