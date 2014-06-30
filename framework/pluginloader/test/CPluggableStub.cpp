#include "CPluggableStub.h"

namespace framework
{
    //-----------------------------------------------------------------------//
    CPluggableStub::CPluggableStub()
    :   IPluggable("CPluggableStub")
    {}

    //-----------------------------------------------------------------------//
    CPluggableStub::~CPluggableStub()
    {}

    //-----------------------------------------------------------------------//
    bool CPluggableStub::loadConfig ( const Json::Value& ar_node )
    {
        LOG4CXX_INFO ( m_logger, "Load Config" );
        return true;
    }

    //-----------------------------------------------------------------------//
    void CPluggableStub::start()
    {
        LOG4CXX_INFO ( m_logger, "Start" );
    }
    
    //-----------------------------------------------------------------------//
    void CPluggableStub::stop()
    {
        LOG4CXX_INFO ( m_logger, "Stop" );
    }
};
