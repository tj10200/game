#ifndef CSceneDB_h
#define CSceneDB_h

#include <string>
#include <vector>
#include <unordered_map>

#include "log4cxx/logger.h"

namespace framework
{
    class IRenderable;
    class IPluggable;

    class CSceneDB
    {
        public: 

            struct SScene
            {
                SScene& operator = ( const SScene& ar_other )
                {
                    m_sceneName = ar_other.m_sceneName;
                    m_renderableLibs = ar_other.m_renderableLibs;
                }
                
                std::string m_sceneName;
                std::vector< std::string > m_renderableLibs;
                std::vector< IPluggable* > m_renderables;
                std::pair< uint16_t, std::string > m_shaderEntry;
            };


            /**
             * Constructor
             *
             */
            CSceneDB ();

            /**
             * Destructor
             */
            ~CSceneDB();

            /**
             * Initialize the scene db
             *
             * @param a_file - the file to load the scene data from
             * @return bool - true on successful load
             */
            bool initialize ( std::string a_file );

            /**
             * Getter for the scene
             *
             * @param a_id - the scene id
             * @param apr_scene - the entry to return
             * @return bool - was the entry found
             */
            bool getScene ( const uint16_t a_id, 
                            SScene*& apr_scene );

            /**
             * Getter for the scene
             *
             * @param ap_name - the name of the scene
             * @param apr_entry - the entry to return
             * @return bool - was the entry found
             */
            bool getScene ( const std::string ar_name, 
                            SScene*& apr_scene );

        private:
            /** The scene array **/
            std::vector < SScene > m_sceneDB; 

            /** The hash map that provides fast lookup using the scene name **/
            std::unordered_map < std::string, uint16_t > m_sceneDBIndex;

            /** The logger object **/
            log4cxx::LoggerPtr m_logger;

    };
};

#endif
