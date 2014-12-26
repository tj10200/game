#ifndef CSimpleTrianglePlugin_h
#define CSimpleTrianglePlugin_h

#include "IRenderable.h"
#include <GL/freeglut.h>

namespace framework
{
    class CSimpleTrianglePlugin: public IRenderable
    {
        public:

            /**
             * Constructor
             */
            CSimpleTrianglePlugin();

            /**
             * Destructor
             */
            virtual ~CSimpleTrianglePlugin();

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
             * Render the triangle object
             */
            virtual void render();

        private:

            /** The number of entries in this triangles vertex buffer **/
            static const uint32_t NUM_VERTICES = 4 * 3 * 2;

            /** The vertex buffer data **/
            static const float ms_vertexData [ NUM_VERTICES ];
        

            /** The vertex array object **/
            GLuint m_vao;

    };
};

#endif
