#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

struct Particle
{
    glm::vec2 Pos = glm::vec2(0, 0);
    glm::vec4 Color = glm::vec4(utils::rand(0, 1), utils::rand(0, 1), utils::rand(0, 1), 1);
};

bool is_far_enough(const glm::vec2& candidate, const std::vector<Particle>& particles, float minDist)
{
    for (const Particle& p : particles)
    {
        if (glm::distance(candidate, p.Pos) < minDist)
            return false;
    }
    return true;
}

int main()
{
    gl::init("Poisson Disk");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    float minDist = 0.08;
    int maxAttempts = 5000;

    std::vector<Particle> particles;

    for (int i = 0; i < maxAttempts; ++i)
    {
        glm::vec2 candidate;
        candidate.x = utils::rand(-gl::window_aspect_ratio(), gl::window_aspect_ratio());
        candidate.y = utils::rand(-1, 1);

        if (is_far_enough(candidate, particles, minDist))
        {
            particles.push_back({ candidate });
        }
    }

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (const Particle& particle : particles)
        {
            utils::draw_disk(particle.Pos, 0.01f, particle.Color);
        }
    }
}