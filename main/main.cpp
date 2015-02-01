#include "CPluginLoader.h"
#include "IPluggable.h"
#include "json/json.h"
#include "gtest/gtest.h"
#include "CGLManager.h"
#include <glload/gl_3_3.h>
#include <glload/gl_load.hpp>
#include <glutil/Shader.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include "CKeyboardPlugin.h"
#include "optionparser.h"
#include "log4cxx/logger.h"
#include <log4cxx/xml/domconfigurator.h>
#include <sstream>

using framework::CPluginLoader;
using framework::IPluggable;

enum EOptions
{
    eOptUnknown,
    eOptHelp,
    eOptConfigFile,
    eOptLoggerConfigFile
};

const option::Descriptor Usage[] =
{
    { eOptUnknown,    0, "", "",        option::Arg::None,                "Usage: main [options] \n\n"
                                                                          "Options: " },
    { eOptHelp,       0, "h", "help",   option::Arg::None,                "    -h,--help    print usage and exit." },
    { eOptConfigFile, 0, "c", "config", option::Arg::NonEmpty,            "    -c,--config  the config file to use for loading initial plugins." },
    { eOptLoggerConfigFile, 0, "x", "log4cxx_config", option::Arg::NonEmpty,  "    -x,--log4cxx_config  the config file to use for log4cxx." },
    { 0,0,0,0,0,0 }
};

struct SOptions 
{
    /** The main config file for starting the application **/
    std::string m_configFile;

    /** The log4cxx config file **/
    std::string m_log4cxxConfigFile;
};

/**
 * Function used to process command line options
 *
 * @param argc - the main's argc
 * @param argv - the main's argv
 * @param ar_options - the populated options struct to return
 * @param ar_logger - the stream to use for options logging
 * @return bool - true if options parsed successfully
 */
bool processOptions ( int argc, 
                      char* argv[], 
                      SOptions& ar_options,
                      std::stringstream& ar_logger )
{
    bool l_ret = true;

    if ( argc == 1 )
    {
        ar_logger << "No options configured" << std::endl;
        l_ret = false;
    }
    else
    {
        --argc;
        ++argv;
    }

    if ( true == l_ret )
    {
        option::Stats l_stats ( Usage, argc, argv );
        option::Option l_options [ l_stats.options_max ];
        option::Option l_buffer [ l_stats.options_max ];
        option::Parser l_parser ( Usage, argc, argv, l_options, l_buffer );
    
        if ( true == l_parser.error() )
        {
            ar_logger << "Failed to parse options" << std::endl;
            l_ret = false; 
        }
        else if ( NULL != l_options[ eOptHelp ] )
        {
            option::printUsage ( ar_logger, Usage );
            l_ret = false;
        }
        
        if ( true == l_ret )
        {
            if ( NULL != l_options [ eOptConfigFile ].arg )
            {
                ar_options.m_configFile = l_options [ eOptConfigFile ].arg;   
                ar_logger << "Configured config file : " << ar_options.m_configFile << std::endl; 
            }
            else
            {
                ar_logger << "Config file missing " << std::endl;
                l_ret = false;
            }

        }

        if ( true == l_ret )
        {
            if ( NULL != l_options [ eOptLoggerConfigFile ].arg )
            {
                ar_options.m_log4cxxConfigFile = l_options [ eOptLoggerConfigFile ].arg;

                ar_logger << "Configured logger config : " << ar_options.m_log4cxxConfigFile << std::endl;
            }
            else
            {
                ar_logger << "log4cxx config file missing " << std::endl;
                l_ret = false;
            }
        }
    }

    return l_ret;
}

int main ( int argc, char* argv[] )
{
    log4cxx::LoggerPtr l_logger = log4cxx::Logger::getLogger( "Main" );
    std::string l_configFile = "";

    {
        std::stringstream l_optionsStream;
        SOptions l_options;

        if ( false == processOptions ( argc, argv, l_options, l_optionsStream ) )
        {
            std::cout << l_optionsStream.str();
            std::cout << "Unable to continue" << std::endl;
            return -1;
        }
        else
        {
            log4cxx::xml::DOMConfigurator::configure( l_options.m_log4cxxConfigFile );
            
            LOG4CXX_INFO ( l_logger, l_optionsStream );
            
            l_configFile = l_options.m_configFile;
        }
    }

    framework::CPluginLoader* lp_loader = CPluginLoader::getInstance();

    lp_loader->loadConfig ( l_configFile );

    framework::IPluggable* lp_keyboard = NULL;
    lp_loader->getPlugin (  framework::CKeyboardPlugin::PLUGIN_ID, lp_keyboard );
    
    lp_loader->startPlugins();

    while ( false == static_cast<framework::CKeyboardPlugin*>(lp_keyboard)->getLeaveMainLoopFlag() )
    {
        lp_loader->updatePlugins();
        glutMainLoopEvent();
    }

    lp_loader->stopPlugins();

    return 0;
}
