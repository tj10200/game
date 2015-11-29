#include "IPluggable.h"
#include "json/json.h"
#include <stdlib.h>

namespace framework
{
    //-----------------------------------------------------------------------//
    IPluggable::IPluggable( const char* ap_name,
                            uint32_t a_id )
    :   m_logger ( log4cxx::Logger::getLogger( ap_name ) ),
        m_rootNode ( "" ),
        m_name ( ap_name ),
        m_id( a_id ),
        m_instanceId ( 0 )
    {
    }

    //-----------------------------------------------------------------------//
    IPluggable::~IPluggable()
    {}

    void IPluggable::update ( const float& ar_elapsedSeconds )
    {
        return;
    }

    //-----------------------------------------------------------------------//
    const std::string& IPluggable::getName()
    {
        return m_name;
    }

    //-----------------------------------------------------------------------//
    uint32_t IPluggable::getId() const
    {
        return m_id;
    }

    //-----------------------------------------------------------------------//
    void IPluggable::setInstanceId( uint32_t a_id )
    {
        m_instanceId = a_id;
    }

    //-----------------------------------------------------------------------//
    uint32_t IPluggable::getInstanceId() const
    {
        return m_instanceId;
    }
};

