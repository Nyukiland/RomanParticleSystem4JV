#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <numbers>

struct ParticleRectangle
{
     glm::vec2 Pos = glm::vec2(
        utils::rand(-gl::window_aspect_ratio() + 0.1, -gl::window_aspect_ratio() + 0.6),
        utils::rand(-0.9,-0.2)
    );
};

struct ParticleParallelogram
{
    float y = utils::rand(-0.9,-0.2);

    glm::vec2 Pos = glm::vec2(
        utils::rand(gl::window_aspect_ratio() - 0.6 + y, gl::window_aspect_ratio() - 0.1 + y),
        y
    );
};

struct ParticleCircle
{
    //pi * r^2 = var_uniforme
    float var = utils::rand(0, 0.4);
    float r = glm::sqrt(var/std::numbers::pi);
    
    float angle = utils::rand(0, 360);

    glm::vec2 Pos = glm::vec2(glm::sin(angle), glm::cos(angle)) * r;
};

int main()
{
    gl::init("Distribution!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    int particleCount = 1000;
    std::vector<ParticleRectangle> particlesRectangle;
    particlesRectangle.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i)
    {
        particlesRectangle.emplace_back();
    }

    std::vector<ParticleParallelogram> particlesParallelogram;
    particlesParallelogram.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i)
    {
        particlesParallelogram.emplace_back();
    }

    std::vector<ParticleCircle> particlesCircle;
    particlesCircle.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i)
    {
        particlesCircle.emplace_back();
    }

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (ParticleRectangle& particle : particlesRectangle)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(1,1,1,1));
        }

        for (ParticleParallelogram& particle : particlesParallelogram)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(1,1,1,1));
        }

        for (ParticleCircle& particle : particlesCircle)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(1,1,1,1));
        }
    }
}