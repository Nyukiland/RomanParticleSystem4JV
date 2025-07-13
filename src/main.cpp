#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <numbers>

struct Particle
{
    float xLeftBound = gl::window_aspect_ratio() - 1;
    float xRightBound = gl::window_aspect_ratio() - 0.5;

    float x = utils::rand(xLeftBound, xRightBound - 0.1);
    float x0to1 = (x - xLeftBound)/(xRightBound - xLeftBound);
    glm::vec2 dir = glm::vec2(0.5, 0.5);
    float ratioDown = 0.5;

    glm::vec2 Pos = glm::vec2(x, 0) + (glm::normalize(dir) * utils::rand(0, x0to1 * ratioDown));
};

struct ParticleEquilateral 
{
    float xLeftBound = -gl::window_aspect_ratio() + 0.1;
    float xRightBound = -gl::window_aspect_ratio() + 0.9;

    float sideLength = xRightBound - xLeftBound;
    float height = sideLength * glm::sqrt(3) / 2;

    float x = utils::rand(xLeftBound, xRightBound);
    float x0to1 = 1 - ((x - xLeftBound) / sideLength);

    glm::vec2 dir = glm::vec2((sideLength/2), height);

    glm::vec2 Pos = glm::vec2(x, 0) + dir * utils::rand(0, x0to1);
};

struct ParticleRectangle
{
    float xLeftBound = -0.3f;
    float xRightBound = 0.3f;

    float x = utils::rand(xLeftBound, xRightBound);
    float sideLength = xRightBound - xLeftBound;

    float x0to1 = 1.f - ((x - xLeftBound) / sideLength); 
    float y = utils::rand(0.f, x0to1 * sideLength);

    glm::vec2 Pos = glm::vec2(x, y);
};



int main()
{
    gl::init("Distribution!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    int particleCount = 1000;
    std::vector<Particle> particles;
    particles.reserve(particleCount);
    for (int x = 0; x < particleCount; ++x)
    {
        particles.emplace_back();       
    }

    std::vector<ParticleEquilateral> particlesEqui;
    particlesEqui.reserve(particleCount);
    for (int x = 0; x < particleCount; ++x)
    {
        particlesEqui.emplace_back();       
    }

    std::vector<ParticleRectangle> particlesRec;
    particlesRec.reserve(particleCount);
    for (int x = 0; x < particleCount; ++x)
    {
        particlesRec.emplace_back();       
    }

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Particle& particle : particles)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(0,1,1,1));
        }

        for (ParticleEquilateral& particle : particlesEqui)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(1,0,1,1));
        }

        for (ParticleRectangle& particle : particlesRec)
        {
            utils::draw_disk(particle.Pos, 0.01, glm::vec4(1,1,0,1));
        }
    }
}