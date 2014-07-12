#include "CEvent.h"
#include <unordered_map>

namespace framework
{
    //-----------------------------------------------------------------------//
    CEvent::CEvent()
    : m_event ( "" )
    {
    }

    //-----------------------------------------------------------------------//
    CEvent::CEvent( const char* ap_event )
    : m_event ( ap_event )
    {
        std::hash< std::string > l_hash;
        setHashCode ( l_hash( m_event ) );
    }

    //-----------------------------------------------------------------------//
    CEvent::~CEvent()
    {
    }

    //-----------------------------------------------------------------------//
    void CEvent::setEvent( const char* ap_event )
    {
        m_event = ap_event;
        computeHash();
    }

    //-----------------------------------------------------------------------//
    const char* CEvent::getEvent()
    {
        return m_event.c_str();
    }

    //-----------------------------------------------------------------------//
    bool CEvent::equals ( const THashable* const & apr_two ) const
    {
        bool l_ret = true;

        //Always true if the pointers are the same
        if ( this != apr_two )
        {
            const CEvent* lp_two = static_cast < const CEvent*> ( apr_two );
        
            l_ret = ( m_event == lp_two->m_event );
        }

        return l_ret;
    }

    //-----------------------------------------------------------------------//
    void CEvent::computeHash()
    {
        if ( false == isHashComputed() )
        {
            std::hash< std::string > l_hash;
            setHashCode ( l_hash( m_event ) );
        }
    }
};
