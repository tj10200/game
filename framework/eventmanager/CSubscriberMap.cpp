#include "CSubscriberMap.h"
#include "CEvent.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    CSubscriberMap::CSubscriberMap()
    {}
    
    //-----------------------------------------------------------------------//
    CSubscriberMap::~CSubscriberMap()
    {
        tEventMapIter l_iter = m_eventMap.begin();

        while ( l_iter != m_eventMap.end() )
        {
            delete l_iter->first;

            ++l_iter;
        }

        m_eventMap.clear();
    }

    //-----------------------------------------------------------------------//
    bool CSubscriberMap::insertEvent ( const char* ap_event,
                                       void*       ap_instance,
                                       tSubscriberCallback a_callback,
                                       CEvent*&    apr_event )
    {
        bool l_ret = true;
        tEventMapIter l_findIter;

        apr_event = new CEvent ( ap_event );
        l_findIter = m_eventMap.find ( apr_event );

        if ( l_findIter != m_eventMap.end() )
        {
            delete apr_event;
            apr_event = static_cast< CEvent* >( l_findIter->first );

            CSubscriberList& lr_insertList = l_findIter->second;
            lr_insertList.insert ( std::make_pair ( (uint64_t)ap_instance, a_callback ) );
        }
        else
        {
            CSubscriberList l_insertList;

            l_insertList.insert ( std::make_pair ( (uint64_t)ap_instance, a_callback ) );    
            m_eventMap.insert ( std::make_pair ( apr_event, l_insertList ) );
        }
            
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CSubscriberMap::removeEvent ( CEvent* ap_event, void* ap_instance )
    {
        bool l_ret = false;
        tEventMapIter l_findIter;

        l_findIter = m_eventMap.find ( ap_event );

        if ( l_findIter != m_eventMap.end() )
        {
            CSubscriberList& lr_list = l_findIter->second;
            CSubscriberList::tSubscriberListIter l_iter = lr_list.begin();

            while ( l_iter != lr_list.end() &&
                    false == l_ret )
            {
                CSubscriberList::tSubscriberListElem& l_elem = (*l_iter);

                if ( l_elem.first == reinterpret_cast<uint64_t>(ap_instance) )
                {
                    l_ret = true;
                    lr_list.erase ( l_iter );
                }

                ++l_iter;
            }
        }
        
        return l_ret;
    }

    //-----------------------------------------------------------------------//
    bool CSubscriberMap::findEvent ( CEvent* ap_event, CSubscriberList*& apr_list )
    {
        bool l_ret = false;
        tEventMapIter l_findIter;

        l_findIter = m_eventMap.find ( ap_event );

        if ( l_findIter != m_eventMap.end() )
        {
            l_ret = true;
            apr_list = &(l_findIter->second);
        }

        return l_ret;
    }
};
