#include "CEventManager.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    CEventManager* CEventManager::sGetInstance()
    {
        static CEventManager ls_instance;

        return &(ls_instance);
    }

    //-----------------------------------------------------------------------//
    bool CEventManager::registerEvent ( const char* ap_event,
                                        void* ap_instance,
                                        tSubscriberCallback a_callback,
                                        CEvent *& apr_event )
    {
        bool l_ret = m_subscriberMap.insertEvent ( ap_event, 
                                                   ap_instance, 
                                                   a_callback,
                                                   apr_event );

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CEventManager::unregisterEvent ( CEvent* ap_event,
                                          void* ap_instance )
    {
        bool l_ret = m_subscriberMap.removeEvent ( ap_event, ap_instance );
    }

    //-----------------------------------------------------------------------//
    bool CEventManager::publishEvent ( CEvent* ap_event,
                                       void* ap_data )
    {
        CSubscriberList* lp_list = NULL;
        bool l_ret = m_subscriberMap.findEvent ( ap_event, lp_list );

        if ( true == l_ret )
        {
            CSubscriberList::tSubscriberListIter l_iter = lp_list->begin();
            
            while ( l_iter != lp_list->end() )
            {
                CSubscriberList::tSubscriberListElem& lr_elem = (*l_iter);
                void* lp_instance = (void*)(lr_elem.first);
                tSubscriberCallback l_callback = lr_elem.second;

                (*l_callback) ( lp_instance, ap_data );

                ++l_iter;
            }
        }

        return l_ret;
    }
};
