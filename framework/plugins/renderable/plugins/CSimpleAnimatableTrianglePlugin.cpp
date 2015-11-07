#include "CSimpleAnimatableTrianglePlugin.h"
#include <GL/freeglut.h>
#include <iostream>

namespace framework
{
    float CSimpleAnimatableTrianglePlugin::ms_vertexData[] =
    {
        0.50f,   0.25f, 0.0f, 1.0f,
        0.75f,  -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
        0.55f,   0.25f, 0.0f, 1.0f,
        0.80f,  -0.75f, 0.0f, 1.0f,
        1.0f,    0.25f, 0.0f, 1.0f,
    };

    //-----------------------------------------------------------------------//
    CSimpleAnimatableTrianglePlugin::CSimpleAnimatableTrianglePlugin()
    :   IRenderable ( "CSimpleAnimatableTrianglePlugin", 2 ),
        m_vao(0),
        m_updated ( false )
    {
    }
    
    //-----------------------------------------------------------------------//
    CSimpleAnimatableTrianglePlugin::~CSimpleAnimatableTrianglePlugin()
    {
    }

    //-----------------------------------------------------------------------//
    bool CSimpleAnimatableTrianglePlugin::loadConfig ( const Json::Value& ar_node )
    {}
    
    //-----------------------------------------------------------------------//
    void CSimpleAnimatableTrianglePlugin::start()
    {
        glGenBuffers(1, &m_vertexBufferHandle);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ms_vertexData), ms_vertexData, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
            
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray( m_vao );
    }

    
    //-----------------------------------------------------------------------//
    void CSimpleAnimatableTrianglePlugin::stop()
    {}
        
    //-----------------------------------------------------------------------//
    void CSimpleAnimatableTrianglePlugin::update( const float& ar_elapsedSeconds )
    {
        static float ls_updateAmount = 0.05f;
        static uint32_t ls_updateTimeUs = 33.0f / 1000.0f; // Update every 33ms
        static timespec ls_lastTime = {0,0};
        static float ls_maxAmount = 5.0f;
        static float ls_direction = 1.0f;

        if ( ( ar_elapsedSeconds - ls_updateTimeUs ) >= ls_updateTimeUs )
        {
            if ( ms_vertexData[0] >= ls_maxAmount ||
                 ms_vertexData[0] <= 0 )
            {
                ls_direction *= -1;
            }
    
            for ( int i = 0; i < 6; ++i )
                *(ms_vertexData + (i * 4) ) += (ls_updateAmount * ls_direction );

            ls_updateTimeUs = ar_elapsedSeconds;

            glutPostRedisplay();
        }
    }

    //-----------------------------------------------------------------------//
    void CSimpleAnimatableTrianglePlugin::render()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);

        glBufferSubData ( GL_ARRAY_BUFFER, 
                          0,
                          sizeof ( ms_vertexData ),
                          ms_vertexData );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);
    }
    
    
};


//-----------------------------------------------------------------------//
extern "C"
{
    framework::IPluggable* createInstance()
    {
        return new framework::CSimpleAnimatableTrianglePlugin;
    }
}
