#include "CPluggableStub.h"

namespace framework
{
    const uint32_t STUB_ID = 1;
    //-----------------------------------------------------------------------//
    CPluggableStub::CPluggableStub()
    :   IPluggable("CPluggableStub", STUB_ID )
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

//-----------------------------------------------------------------------//
extern "C"
{
    framework::IPluggable* createInstance()
    {
        return new framework::CPluggableStub;
    }
}
