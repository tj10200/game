#include "IPluggable.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    IPluggable::IPluggable( const char* ap_name )
    :   m_logger ( log4cxx::Logger::getLogger( ap_name ) ),
        m_rootNode ( "" ),
        m_name ( ap_name )
    {}

    //-----------------------------------------------------------------------//
    IPluggable::~IPluggable()
    {}

    //-----------------------------------------------------------------------//
    const std::string& IPluggable::getName()
    {
        return m_name;
    }
};

