#ifndef CSceneManagerPlugin_h
#define CSceneManagerPlugin_h

#include <string>
#include <unordered_map>

#include "CPluginLoader.h"
#include "IPluggable.h"

#include "log4cxx/logger.h"

namespace framework
{
    class CEvent;
    class IRenderable;
    class CSceneDB;

    class CSceneManagerPlugin: public IPluggable
    {
        public:
            static const uint32_t SCENE_MANAGER_PLUGIN_ID;

            /**
             * Constructor
             *
             * @param ap_name - the name of this object
             */
            CSceneManagerPlugin();
    
            /**
             * Destructor
             */
            virtual ~CSceneManagerPlugin();
    
            /**
             * Loads configuration data
             * 
             * @param ar_node - the root node for this object
             * @return bool - true on successful config load
             */
            virtual bool loadConfig ( const Json::Value& ar_node );
    
            /**
             * Starts the object
             */
            virtual void start();
    
            /**
             * Stops the object
             */
            virtual void stop();

            /**
             * Register a scene as being active. This will register all
             * renderable libs with the scene manager
             *
             * @param ap_instance - the scene manager instance
             * @param ap_data - a scene
             */
            static void sRegisterSceneCallback ( void* ap_instance,
                                                 void* ap_data );
            void registerSceneCallback ( uint16_t* ap_data );
            
            /**
             * Deregister a scene of IRenderable objects from a scene program
             *
             * @param ap_instance - the scene manager instance
             * @param ap_data - a tSceneRegistrationEvent
             */
            static void sDeregisterSceneCallback ( void* ap_instance,
                                                   void* ap_data );
            void deregisterSceneCallback ( uint16_t* ap_data );

        private:

            /** The Register Scene Callback Event object **/
            CEvent* mp_loadSceneCallbackEvent;

            /** The Deregister Scene Callback Event Object **/
            CEvent* mp_unloadSceneCallbackEvent;

            /** The scene DB **/
            CSceneDB* mp_sceneDB;
    };
};
 
#endif
