#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <numbers>

struct Particle
{
    glm::vec2 Pos = glm::vec2(0, 0);
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
    float u = 1.0f - t;
    float b0 = u * u;
    float b1 = 2 * u * t;
    float b2 = t * t;

    return b0 * p0 + b1 * p1 + b2 * p2;
}


glm::vec2 bezier3(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3, float t)
{
    float u = 1.0f - t;
    float b0 = u * u * u;
    float b1 = 3 * u * u * t;
    float b2 = 3 * u * t * t;
    float b3 = t * t * t;

    return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}


int main()
{
    gl::init("Courbes");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_parametric([](float t) {
            return bezier1({-0.5f, -0.5f}, {-0.5f, 0.5f}, t);
        }, glm::vec4(1,1,1,1));

         draw_parametric([](float t) {
            return bezier2({-0.3f, 0.5f}, gl::mouse_position(), {0.8f, 0.5f}, t);
        }, glm::vec4(0,1,1,1));

         draw_parametric([](float t) {
            return bezier3({-0.3f, -0.3f}, {0, 0}, gl::mouse_position(), {0.8f, -0.3f}, t);
        }, glm::vec4(1,0,1,1));
    }
}