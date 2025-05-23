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

        glm::vec2 ForceDir = glm::vec2(
            utils::rand(-1, 1),
            utils::rand(-1,1)
        );

        float Speed = utils::rand(0, 1);
        float Mass = utils::rand(1, 5);
    };
    

    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    bool inverse = false;
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

        if (time > 1) inverse = true;
        else if (time < 0.1) inverse = false;

        if (inverse) time -= gl::delta_time_in_seconds() / 2;
        else time += gl::delta_time_in_seconds() / 2;

        for (Particle& particle : particles)
        {
            glm::vec2 toApply = glm::normalize(particle.ForceDir) * gl::delta_time_in_seconds() * particle.Speed;

            toApply += particle.Mass * 0.9f * gl::delta_time_in_seconds() * glm::vec2(0, -1);

            particle.Pos += toApply;
            
            utils::draw_disk(particle.Pos, time/100, glm::vec4(1,1,1,1));

            if (glm::abs(particle.Pos.x) > glm::abs(gl::window_aspect_ratio())) particle.ForceDir.x *= -1;
            if (glm::abs(particle.Pos.y) > 1) particle.ForceDir.y *= -1;
        }

        // TODO update particles
        // TODO render particles
    }
}