#ifndef RenderableManagerTypes_h
#define RenderableManagerTypes_h

namespace framework
{
    typedef IPluggable* (tRenderableInstanceFunc) ();

    struct SScene
    {
        std::string m_name;
        std::vector< tRenderableInstanceFunc > m_libs;
    };
};

#endif
