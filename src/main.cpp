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
        Origin = O;
    };

    glm::vec2 Origin;
    glm::vec2 EndPoint;
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

    int particleCount = 10;
    std::vector<Particle> particles;
    particles.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i)
    {
        particles.emplace_back();
    }

    std::vector<LineStruct> Lines;
    std::vector<CircleStruct> Circles;
    
    int pressed = -1;

    gl::set_events_callbacks({
        {
            .on_mouse_moved = [&](gl::MouseMoveEvent const& e){
                if (pressed == 0)
                {
                    glm::vec2 point = gl::mouse_position();
                    Lines[Lines.size() - 1].EndPoint = point;
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
                    Lines[Lines.size() - 1].EndPoint = point;
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

        for (LineStruct& obj : Lines)
        {
            utils::draw_line(obj.Origin, obj.EndPoint, 0.02, glm::vec4(0.5, 0.5, 0.5, 1));
        }
        
        for (CircleStruct& circle : Circles)
        {
            utils::draw_disk(circle.Origin, circle.Size, glm::vec4(0.5, 0.5, 0.5, 1));
        }

        for (Particle& particle : particles)
        {
            glm::vec2 toApply = glm::normalize(particle.ForceDir) * gl::delta_time_in_seconds() * particle.Speed;

            //toApply += particle.Mass * 0.9f * gl::delta_time_in_seconds() * glm::vec2(0, -1);

            //toApply += (gl::mouse_position() - particle.Pos) * gl::delta_time_in_seconds() * (particle.Mass /10);

            Vector2D vec(particle.Pos, glm::normalize(toApply), glm::length(toApply) + particle.LifeSize);

            for (LineStruct& obj : Lines)
            {
                glm::vec2 dir(obj.EndPoint - obj.Origin);
                Vector2D vecL(obj.Origin, glm::normalize(dir), glm::length(dir));
                if (GetIntersectionPoint(vec, vecL) != glm::vec2(-123.0f, -123.0f)) 
                {
                    toApply = glm::reflect(toApply, glm::vec2(-dir.y, dir.x));
                    particle.ForceDir = glm::reflect(particle.ForceDir, glm::vec2(dir.y, dir.x));
                }
            }

            for (CircleStruct& circle : Circles)
            {
                glm::vec2 toCenter = particle.Pos - circle.Origin;
                float dist = glm::length(toCenter);

                if (dist <= circle.Size + particle.LifeSize)
                {
                    glm::vec2 normal = glm::normalize(toCenter);
                    toApply = glm::reflect(toApply, normal);
                    particle.ForceDir = glm::reflect(particle.ForceDir, normal);

                    particle.Pos = circle.Origin + normal * (circle.Size + particle.LifeSize + 0.001f);
                }
            }


            particle.Pos += toApply;

            utils::draw_disk(particle.Pos, particle.LifeSize, particle.ColorEnd);

            if (glm::abs(particle.Pos.x) > glm::abs(gl::window_aspect_ratio())) particle.ForceDir.x *= -1;
            if (glm::abs(particle.Pos.y) > 1) particle.ForceDir.y *= -1;
        }

        // TODO update particles
        // TODO render particles
    }
}