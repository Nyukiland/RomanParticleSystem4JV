#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

struct Vector2D
{
    Vector2D(glm::vec2 O, glm::vec2 D, float M)
    {
        Origin = O;
        Dir = D;
        Magnitude = M;
    };

    glm::vec2 Origin;
    glm::vec2 Dir;
    float Magnitude;
};

struct LineStruct
{
    LineStruct(glm::vec2 O)
    {
        Points.emplace_back(O);
    };

    std::vector<glm::vec2> Points;
};

struct CircleStruct
{
    CircleStruct(glm::vec2 O)
    {
        Origin = O;
    }

    glm::vec2 Origin;
    float Size;
};

glm::vec2 GetIntersectionPoint(const Vector2D& Vec1, const Vector2D& Vec2)
{
    glm::vec2 OriginVector = Vec2.Origin - Vec1.Origin;

    glm::mat2 M(Vec1.Dir, -Vec2.Dir);

    float det = glm::determinant(M);
    if (glm::abs(det) < 1e-6f) 
    {
        return glm::vec2(-123.0f, -123.0f); //return value if no intersection
    }

    glm::vec2 t = glm::inverse(M) * OriginVector;

    if (t.x < 0.0f || t.x > Vec1.Magnitude || t.y < 0.0f || t.y > Vec2.Magnitude) 
    {
        return glm::vec2(-123.0f, -123.0f); //return value if no intersection
    }

    return Vec1.Origin + t.x * Vec1.Dir;
}

bool LineIntersectsCircle(glm::vec2 A, glm::vec2 B, glm::vec2 center, float radius)
{
    glm::vec2 AB = B - A;
    glm::vec2 AC = center - A;

    float t = glm::dot(AC, AB) / glm::dot(AB, AB);
    t = glm::clamp(t, 0.0f, 1.0f);

    glm::vec2 closest = A + t * AB;

    float distance = glm::length(closest - center);
    return distance <= radius;
}


int main()
{   
    gl::init("Rope!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    std::vector<LineStruct> Lines;
    std::vector<CircleStruct> Circles;
    
    int pressed = -1;

    gl::set_events_callbacks({
        {
            .on_mouse_moved = [&](gl::MouseMoveEvent const& e){
                if (pressed == 0)
                {
                    glm::vec2 point = gl::mouse_position();
                    int lastIndex = Lines.back().Points.size() - 1;
                    glm::vec2 lastPoint = Lines.back().Points[lastIndex - 1]; // second to last

                    bool collided = false;

                    for (const auto& circle : Circles)
                    {
                        if (LineIntersectsCircle(lastPoint, point, circle.Origin, circle.Size))
                        {
                            glm::vec2 dir = glm::normalize(point - circle.Origin);
                            glm::vec2 tangentPoint = circle.Origin + dir * (circle.Size + 0.02f); 

                            Lines.back().Points.insert(Lines.back().Points.end() - 1, tangentPoint);

                            collided = true;
                            break;
                        }
                    }

                    // Always update the last point to current mouse
                    Lines.back().Points.back() = point;
                }
                else if (pressed == 1)
                {
                    float dist = glm::length(gl::mouse_position() - Circles[Circles.size() - 1].Origin);
                    Circles[Circles.size() - 1].Size = dist;
                }
            },
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e){
                if (e.button == 0) 
                {
                    Lines.emplace_back(gl::mouse_position());
                    Lines[Lines.size() - 1].Points.emplace_back(gl::mouse_position());
                }
                else if (e.button == 1)
                {
                    Circles.emplace_back(gl::mouse_position());
                }
                pressed = e.button;
            },
            .on_mouse_released = [&](gl::MouseReleasedEvent const& e){
                if (pressed == 0)
                {
                    glm::vec2 point = gl::mouse_position();
                    int LastIndex = Lines[Lines.size() - 1].Points.size() -1;
                    Lines[Lines.size() - 1].Points[LastIndex] = point;
                }
                else if (pressed == 1)
                {
                    float dist = glm::length(gl::mouse_position() - Circles[Circles.size() - 1].Origin);
                    Circles[Circles.size() - 1].Size = dist;
                }
                pressed = -1;
            }
        }
    });


    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (LineStruct& line : Lines)
        {
            for(int i = 0; i < line.Points.size() - 1; i++)
            {
                utils::draw_line(line.Points[i], line.Points[i+1], 0.02, glm::vec4(0, 0, 1, 1));
            }
        }
        
        for (CircleStruct& circle : Circles)
        {
            utils::draw_disk(circle.Origin, circle.Size, glm::vec4(0.5, 0.5, 0.5, 1));
        }
    }
}