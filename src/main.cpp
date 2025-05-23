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


        glm::vec4 ColorEnd = glm::vec4(
            utils::rand(0, 1),
            utils::rand(0, 1),
            utils::rand(0, 1),
            1
        );

         glm::vec4 ColorStart = glm::vec4(1, 1, 1, 1);

        float Speed = utils::rand(0.1, 1);
        float Mass = utils::rand(1, 5);
        float LifeSize = utils::rand(0.08f, 0.15f);
        float LifeMore = utils::rand(1, 3);
        float LifeTotal = LifeMore + LifeSize;
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

            if (particle.LifeMore < 0)
            {
                particle.LifeSize -= gl::delta_time_in_seconds() / (particle.LifeSize * 1000);
                particle.LifeSize = glm::clamp(particle.LifeSize, 0.0f, 10.0f);
            }
            else particle.LifeMore -= gl::delta_time_in_seconds();
            
            float t = (particle.LifeMore + particle.LifeSize) / particle.LifeTotal;
            glm::vec4 LerpColor = particle.ColorEnd + (particle.ColorStart - particle.ColorEnd) * t;

            utils::draw_disk(particle.Pos, particle.LifeSize, LerpColor);

            if (glm::abs(particle.Pos.x) > glm::abs(gl::window_aspect_ratio())) particle.ForceDir.x *= -1;
            if (glm::abs(particle.Pos.y) > 1) particle.ForceDir.y *= -1;
        }

        std::erase_if(particles, [](Particle const& x) { return x.LifeSize <= 0;});

        // TODO update particles
        // TODO render particles
    }
}