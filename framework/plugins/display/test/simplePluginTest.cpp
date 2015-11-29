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
    const framework::CPluginLoader::tPluginSet& l_plugins = lp_loader->getPlugins();

    EXPECT_EQ ( l_plugins.size(), 5 );

    EXPECT_TRUE ( false == lp_loader->getPlugin( 3, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( true == lp_loader->getPlugin( 1404222126, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( NULL != lp_searchPlugin );

    framework::IPluggable* lp_keyboard = NULL;
    lp_loader->getPlugin (  framework::CKeyboardPlugin::PLUGIN_ID, 0, lp_keyboard );
    
    lp_loader->startPlugins();

    EXPECT_EQ ( l_plugins.size(), 6 );
    EXPECT_TRUE ( true == lp_loader->getPlugin( 2, 0, lp_searchPlugin ) );
    EXPECT_TRUE ( NULL != lp_searchPlugin );

    while ( false == static_cast<framework::CKeyboardPlugin*>(lp_keyboard)->getLeaveMainLoopFlag() )
    {
        lp_loader->updatePlugins();
        glutMainLoopEvent();
    }

    lp_loader->stopPlugins();
}
