#include "log4cxx/logger.h"

using namespace log4cxx;
int main()
{

    log4cxx::LoggerPtr l_myLogger ( log4cxx::Logger::getLogger ( "Random" ) );

    LOG4CXX_INFO( l_myLogger, "Basic INFO String" );
    
    return 0;
}
