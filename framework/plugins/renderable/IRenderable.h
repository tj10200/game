#ifndef IRenderable_h
#define IRenderable_h

#include "IPluggable.h"
#include <GL/freeglut.h>
#include "THashable.h"

namespace framework
{
    class IRenderable: public IPluggable, THashable<uint32_t>
    {
        public:
            /**
             * Constructor
             *
             * @param ap_name - the name of the plugin
             * @parma a_id - the plugin ID
             */
            IRenderable(const char* ap_name,
                        const uint16_t a_id  );

            /**
             * Destructor
             */
            virtual ~IRenderable();

            /**
             * Renders the object
             */
            virtual void render() =0;

            /**
             * Inherited from base class
             */
            virtual void computeHash();

            /**
             * Compares the names of the objects
             */
            virtual bool equals ( const THashable* const & apr_two ) const;

            /**
             * Updates the renderable item
             *
             * @param ar_time - the current time
             */
            virtual void update ( const timespec& ar_time );

        protected:

            /** The vertex buffer handle **/
            GLuint  m_vertexBufferHandle;
        
    };
};

#endif
