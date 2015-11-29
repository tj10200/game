#ifndef SceneManagerCommon_h
#define SceneManagerCommon_h

#include <map>
#include <string>
#include "GL/freeglut.h"

namespace framework
{
    class IRenderable;

    static const char* const REGISTER_SCENE_EVENT          = "register_scene";
    static const char* const DEREGISTER_SCENE_EVENT        = "deregister_scene";

};

#endif
