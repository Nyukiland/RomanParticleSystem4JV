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

glm::vec2 find_closest(std::function<glm::vec2(float)> const& parametric, glm::vec2 targetPos, int samples = 500)
{
    float best_dist2 = 1000000; 
    glm::vec2 bestPoint = glm::vec2(1000, 1000);

    for (int i = 0; i <= samples; ++i)
    {
        float t = (float)i / samples;
        glm::vec2 point = parametric(t);
        float dist2 = glm::length(point - targetPos); 

        if (dist2 < best_dist2)
        {
            best_dist2 = dist2;
            bestPoint = point;
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

    auto bezierCurve = [](float t) {
        return bezier3({-0.7f, 0}, {0, 0}, {0.5f, 0.5f}, {0.8f, -0.3f}, t);
    };


    int particlesCount = 20;
    std::vector<Particle> particles;
    
    for (int i = 0; i < particlesCount; ++i)
    {
        float placement = (float)i/(float)particlesCount;
        
        glm::vec2 pos = bezierCurve(placement);
        glm::vec2 normal = find_normal(bezierCurve, placement);
        
        particles.push_back({ pos, normal });
    }
    
    float speed = 0.2f;

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

         draw_parametric(bezierCurve, glm::vec4(1,1,1,1));

        for (Particle& particle : particles)
        {
            particle.Pos += glm::normalize(particle.Dir) * gl::delta_time_in_seconds() * speed;
            utils::draw_disk(particle.Pos, 0.01f, particle.Color);
        }
        
        glm::vec2 closest = find_closest(bezierCurve, gl::mouse_position());
        utils::draw_line(closest, gl::mouse_position(), 0.01f, glm::vec4(0,0,1,1));
        utils::draw_disk(closest, 0.025f, glm::vec4(1,0,0,1));
    }
}