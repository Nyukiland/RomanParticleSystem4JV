#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <numbers>

struct Particle
{
     glm::vec2 Pos = glm::vec2(0,0);
};

int main()
{
    gl::init("Distribution!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    float spacing = 0.1;

    float xCount = (gl::window_aspect_ratio()*2)/spacing;
    float yCount = 2/spacing;

    int index = 0;

    int particleCount = xCount * yCount;
    std::vector<Particle> particles;
    particles.reserve(particleCount);
    for (int x = 0; x < xCount; ++x)
    {
        for (int y = 0; y < yCount; ++y)
        {
            particles.emplace_back();
            
            particles[index].Pos = glm::vec2(-gl::window_aspect_ratio() + spacing*x, -1 + spacing*y);
            index++;
        }
    }


    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Particle& particle : particles)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(1,1,1,1));
        }
    }
}