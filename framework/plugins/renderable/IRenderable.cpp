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
    {
    }

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

        l_ret = ( m_id == lp_two->m_id );

        return l_ret;

    }

    //-----------------------------------------------------------------------//
    void IRenderable::update( const timespec& ar_time )
    {
       m_preChildren.visit ( IRenderable::sVisitorUpdate, ar_time ); 
       m_postChildren.visit ( IRenderable::sVisitorUpdate, ar_time ); 
    }

    //-----------------------------------------------------------------------//
    void IRenderable::addPreChild ( IRenderable* ap_child )
    {
        m_preChildren.insert ( ap_child );
    }

    //-----------------------------------------------------------------------//
    IRenderable* IRenderable::getPreChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::removePreChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    void IRenderable::addPostChild ( IRenderable* ap_child )
    {
    }

    //-----------------------------------------------------------------------//
    IRenderable* IRenderable::getPostChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    bool IRenderable::removePostChild ( uint32_t a_id )
    {
    }

    //-----------------------------------------------------------------------//
    void IRenderable::sVisitorUpdate ( IRenderable* ap_obj, const timespec& ar_data )
    {
        ap_obj->update ( ar_data );
    }

}
