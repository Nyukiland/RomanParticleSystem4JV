#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

int main()
{
    struct Particle
    {
        glm::vec2 Pos = glm::vec2(
            utils::rand(-gl::window_aspect_ratio(), gl::window_aspect_ratio()),
            utils::rand(-1,1)
        );
    };
    

    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    float time = 0;

    int particleCount = 100;

    std::vector<Particle> particles;
    particles.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i)
    {
        particles.emplace_back();
    }

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (time < 1) time += gl::time_in_seconds() / 10;
        else time -= gl::time_in_seconds() / 10;

        for (Particle particle : particles)
        {
            
            utils::draw_disk(particle.Pos, time/100, glm::vec4(1,1,1,1));
        }

        // TODO update particles
        // TODO render particles
    }
}