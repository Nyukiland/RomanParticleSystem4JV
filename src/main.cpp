#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <numbers>

struct Particle
{
    glm::vec2 Pos = glm::vec2(0, 0);
    glm::vec4 Color = glm::vec4(utils::rand(0, 1), utils::rand(0, 1), utils::rand(0, 1), 1);
};

void draw_parametric(std::function<glm::vec2(float)> const& parametric)
{
    const int segments = 500;
    glm::vec2 previousPoint = parametric(0.0f);  // Premier point à t = 0

    for (int i = 1; i <= segments; ++i)
    {
        float t = static_cast<float>(i) / segments;
        glm::vec2 currentPoint = parametric(t);

        utils::draw_line(previousPoint, currentPoint, 0.01f, glm::vec4(1,1,1,1));

        previousPoint = currentPoint;
    }
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
            float angle = t * 2 * std::numbers::pi;
            return glm::vec2(std::cos(angle)/2, std::sin(angle)/2);
        });
    }
}