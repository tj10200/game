#include "IPluggable.h"
#include "json/json.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    IPluggable::IPluggable( const char* ap_name,
                            uint32_t a_id )
    :   m_logger ( log4cxx::Logger::getLogger( ap_name ) ),
        m_rootNode ( "" ),
        m_name ( ap_name ),
        m_id( a_id )
    {}

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
    const uint32_t IPluggable::getId()
    {
        return m_id;
    }
};

