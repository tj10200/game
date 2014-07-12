#ifndef CEvent_h
#define CEvent_h

#include "THashable.h"
#include <stdint.h>
#include <string>

namespace framework
{
    class CEvent: public THashable<uint32_t>
    {
        public:

            /**
             * Constructor
             */
            CEvent();
            
            /**
             * Constructor
             *
             * @param ap_event - the event to create
             */
            CEvent( const char* ap_event );

            /**
             * Destructor
             */
            virtual ~CEvent();

            /**
             * Sets the event name
             *
             * @param ap_event - the event to set
             */
            void setEvent ( const char* ap_event );

            /**
             * Gets the event
             */
            const char* getEvent ();

            /**
             * The Equality operator
             *
             * @param apr_two - the object to compare against
             */
            virtual bool equals ( const THashable* const & apr_two ) const;
           
            /**
             * The hash computer
             */
            virtual void computeHash();

        private:

            /** The Event Name **/
            std::string m_event;
    };
};

#endif
