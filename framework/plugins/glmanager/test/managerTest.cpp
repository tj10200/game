#include "CPluginLoader.h"
#include "IPluggable.h"
#include "json/json.h"
#include "gtest/gtest.h"
#include <fstream>

using framework::CPluginLoader;
using framework::IPluggable;

TEST ( PluginTest, main)
{
    framework::CPluginLoader l_loader;
    framework::IPluggable* lp_plugin = NULL;

    EXPECT_EQ ( l_loader.createPlugin( "../libglmanagerplugin.so", lp_plugin ),
                0);
    
    ASSERT_TRUE ( NULL != lp_plugin );

    Json::Value l_rootNode; 
    Json::Reader l_reader;
    std::ifstream l_testFile ( "./managerTestConfig.json", std::ifstream::binary);

    ASSERT_TRUE ( l_reader.parse( l_testFile, l_rootNode ) );
    ASSERT_TRUE ( lp_plugin->loadConfig( l_rootNode ) );

    lp_plugin->start();
}
