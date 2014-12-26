#include "IRenderable.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    IRenderable::IRenderable ( const char* ap_name,
                               const uint16_t a_id )
    :   IPluggable ( ap_name, a_id ),
        m_vertexBufferHandle ( 0 )
    {}

    //-----------------------------------------------------------------------//
    IRenderable::~IRenderable()
    {}

    //-----------------------------------------------------------------------//
    void IRenderable::computeHash()
    {
        setHashCode ( m_id );
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::equals ( const THashable* const & apr_two ) const
    {
        bool l_ret = true;

        IRenderable* lp_two = (IRenderable*)apr_two;

        l_ret = ( m_name == lp_two->m_name );

        return l_ret;

    }

    //-----------------------------------------------------------------------//
    void IRenderable::update( const timespec& ar_time )
    {
        return;
    }
}
