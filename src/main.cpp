#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

int main()
{
    gl::init("Distribution!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);




    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        
    }
}