#ifndef GLManagerEvents_h
#define GLManagerEvents_h

namespace framework
{
    /** 
     * GLUT Display event
     */
    struct SDisplayEvent
    {
    };

    /** 
     * GLUT Reshape event 
     *
     * @param a_width - the new window width
     * @param a_height - the new window height
     */
    struct SReshapeEvent
    {
        int m_width;
        int m_height;
    };

    /**
     * GLUT Keyboard function event
     *
     * @param a_key - the key that was pressed
     * @param a_mouseX - the mouse X location
     * @param a_mouseY - the mouse Y location
     */
    struct SKeyboardEvent
    {
        unsigned char m_key;
        int m_mouseX;
        int m_mouseY;
    };
    
};

#endif
