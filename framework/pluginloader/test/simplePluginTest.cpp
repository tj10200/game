#include "CPluginLoader.h"
#include "IPluggable.h"
#include "json/json.h"
#include "gtest/gtest.h"
#include "CGLManager.h"

using framework::CPluginLoader;
using framework::IPluggable;

TEST ( PluginTest, main)
{
    framework::CPluginLoader* lp_loader = CPluginLoader::getInstance();
    std::string l_configFile = "./pluginLoaderTestConfig.json";
    IPluggable* lp_searchPlugin = NULL;

    EXPECT_TRUE ( false == lp_loader->getPlugin( 1, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( false == lp_loader->getPlugin( 1404222126, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( lp_loader->loadConfig ( l_configFile ) );
    framework::CPluginLoader::tPluginSet l_plugins = lp_loader->getPlugins();

    EXPECT_EQ ( l_plugins.size(), 1 );
    EXPECT_TRUE ( false == lp_loader->getPlugin( 1, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( true == lp_loader->getPlugin( 1404222126, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( true == lp_loader->getPlugin( 1404222126, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( NULL != lp_searchPlugin );
    
    lp_loader->startPlugins();

    
    
}
