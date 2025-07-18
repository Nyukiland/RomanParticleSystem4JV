#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <numbers>

struct Particle
{
    glm::vec2 Pos = glm::vec2(0, 0);
    glm::vec2 Dir = glm::vec2(0, 0);
    glm::vec4 Color = glm::vec4(utils::rand(0, 1), utils::rand(0, 1), utils::rand(0, 1), 1);
};

void draw_parametric(std::function<glm::vec2(float)> const& parametric, glm::vec4 color)
{
    const int segments = 500;
    glm::vec2 previousPoint = parametric(0.0f);  // Premier point à t = 0

    for (int i = 1; i <= segments; ++i)
    {
        float t = static_cast<float>(i) / segments;
        glm::vec2 currentPoint = parametric(t);

        utils::draw_line(previousPoint, currentPoint, 0.01f, color);

        previousPoint = currentPoint;
    }
}

glm::vec2 lerp(const glm::vec2 a, const glm::vec2 b, float t) {
    return (1.0f - t) * a + t * b;
}

glm::vec2 bezier1(const glm::vec2 p0, const glm::vec2 p1, float t)
{
    return lerp(p0, p1, t);
}

glm::vec2 bezier2(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, float t)
{
    glm::vec2 A = lerp(p0, p1, t);
    glm::vec2 B = lerp(p1, p2, t);

    return lerp(A, B, t);
}


glm::vec2 bezier3(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3, float t)
{
    glm::vec2 A = lerp(p0, p1, t);
    glm::vec2 B = lerp(p1, p2, t);
    glm::vec2 C = lerp(p2, p3, t);

    glm::vec2 D = lerp(A, B, t);
    glm::vec2 E = lerp(B, C, t);

    return lerp(D, E, t);
}

glm::vec2 find_normal(std::function<glm::vec2(float)> const& parametric, float t)
{
    glm::vec2 pos = parametric(t);
    glm::vec2 pos2 = parametric(t + 0.02);
        
    glm::vec2 tangent = pos2 - pos;
    
    return glm::normalize(glm::vec2(-tangent.y, tangent.x));
}

glm::vec2 find_closest(std::function<glm::vec2(float)> const& parametric, glm::vec2 targetPos, float& outT, float& outDist, int samples = 500)
{
    outDist = 1000000; 
    glm::vec2 bestPoint = glm::vec2(1000, 1000);

    for (int i = 0; i <= samples; ++i)
    {
        float t = (float)i / samples;
        glm::vec2 point = parametric(t);
        float dist2 = glm::length(point - targetPos); 

        if (dist2 < outDist)
        {
            outDist = dist2;
            bestPoint = point;
            outT = t;

        }
    }

    return bestPoint;
}

int main()
{
    gl::init("Courbes");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    auto shape = [](float t) {
        float theta = t * 2.0f * std::numbers::pi;
        float r = 1.0f - sin(theta);
        float x = r * cos(theta);
        float y = r * sin(theta);
        return glm::vec2(x, y) * 0.5f;
    };

    int particlesCount = 100;
    std::vector<Particle> particles;
    
    for (int i = 0; i < particlesCount; ++i)
    {
        float placement = (float)i/(float)particlesCount;
        
        glm::vec2 pos = lerp(glm::vec2(-gl::window_aspect_ratio(), 1), glm::vec2(gl::window_aspect_ratio(), 1), placement);
        
        particles.push_back({ pos, glm::vec2(0,-1) });
    }
    
    float gravity = 0.002f;
    float forceFieldMultiplier = 3.0f;

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

         draw_parametric(shape, glm::vec4(1,1,1,1));

        for (Particle& particle : particles)
        {
            float closestT;
            float closestDist;
            glm::vec2 closest = find_closest(shape, particle.Pos, closestT, closestDist);

           closestDist = glm::clamp(closestDist, 0.001f, 1.0f);

            float forceStrength = (1.0f - closestDist);
            forceStrength *= forceStrength;

            glm::vec2 forceCurve = forceStrength * find_normal(shape, closestT) * forceFieldMultiplier;

            particle.Pos += particle.Dir * gl::delta_time_in_seconds();
            particle.Dir += (glm::vec2(0, -gravity) + forceCurve) * gl::delta_time_in_seconds();
            
            utils::draw_line(particle.Pos, closest, 0.005f, glm::vec4(0,0,1,1));
            utils::draw_disk(particle.Pos, 0.01f, particle.Color);
        }
    }
}