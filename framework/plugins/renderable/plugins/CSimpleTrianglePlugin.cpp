#include "CSimpleTrianglePlugin.h"
#include <GL/freeglut.h>

namespace framework
{
    const float CSimpleTrianglePlugin::ms_vertexData[] =
    {
        0.50f,   0.25f, 0.0f, 1.0f,
        0.75f,  -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
        0.55f,   0.25f, 0.0f, 1.0f,
        0.80f,  -0.75f, 0.0f, 1.0f,
        1.0f,    0.25f, 0.0f, 1.0f,
    };

    //-----------------------------------------------------------------------//
    CSimpleTrianglePlugin::CSimpleTrianglePlugin()
    :   IRenderable ( "CSimpleTrianglePlugin", 1 ),
        m_vao(0)
    {
    }
    
    //-----------------------------------------------------------------------//
    CSimpleTrianglePlugin::~CSimpleTrianglePlugin()
    {
    }

    //-----------------------------------------------------------------------//
    bool CSimpleTrianglePlugin::loadConfig ( const Json::Value& ar_node )
    {}
    
    //-----------------------------------------------------------------------//
    void CSimpleTrianglePlugin::start()
    {
        glGenBuffers(1, &m_vertexBufferHandle);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ms_vertexData), ms_vertexData, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0 );
            
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray( m_vao );
    }

    
    //-----------------------------------------------------------------------//
    void CSimpleTrianglePlugin::stop()
    {}
        
    //-----------------------------------------------------------------------//
    void CSimpleTrianglePlugin::render()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
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
        return new framework::CSimpleTrianglePlugin;
    }
}
