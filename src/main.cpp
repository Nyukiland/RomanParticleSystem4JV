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
        float LifeSize = utils::rand(0.08f, 0.15f);
    };
    

    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    bool inverse = false;

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

        for (Particle& particle : particles)
        {
            glm::vec2 toApply = glm::normalize(particle.ForceDir) * gl::delta_time_in_seconds() * particle.Speed;

            //toApply += particle.Mass * 0.9f * gl::delta_time_in_seconds() * glm::vec2(0, -1);

            toApply += (gl::mouse_position() - particle.Pos) * gl::delta_time_in_seconds() * (particle.Mass /10);

            particle.Pos += toApply;

            particle.LifeSize -= gl::delta_time_in_seconds() / (particle.LifeSize * 1000);
            particle.LifeSize = glm::clamp(particle.LifeSize, 0.0f, 10.0f);
            
            utils::draw_disk(particle.Pos, particle.LifeSize, glm::vec4(1,1,1,1));

            if (glm::abs(particle.Pos.x) > glm::abs(gl::window_aspect_ratio())) particle.ForceDir.x *= -1;
            if (glm::abs(particle.Pos.y) > 1) particle.ForceDir.y *= -1;
        }

        std::erase_if(particles, [](Particle const& x) { return x.LifeSize <= 0;});

        // TODO update particles
        // TODO render particles
    }
}