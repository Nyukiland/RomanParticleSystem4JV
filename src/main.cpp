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

struct ObjectData
{
    ObjectData(glm::vec2 O)
    {
        Origin = O;
    };

    glm::vec2 Origin;
    glm::vec2 EndPoint;
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

    std::vector<ObjectData> Objects;

    std::vector<Particle> particles;
    particles.reserve(particleCount);
    for (int i = 0; i < particleCount; ++i)
    {
        particles.emplace_back();
    }

    gl::set_events_callbacks({
        {
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e){
                Objects.emplace_back(gl::mouse_position());
            },
            // .on_mouse_moved = [&](gl::MouseMoveEvent const& e){
            //     glm::vec2 point = gl::mouse_position();
            //     Objects[Objects.size() - 1].EndPoint = point;
            // },
            .on_mouse_released = [&](gl::MouseReleasedEvent const& e){
                glm::vec2 point = gl::mouse_position();
                Objects[Objects.size() - 1].EndPoint = point;
            }
        }
    });


    while (gl::window_is_open())
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (ObjectData& obj : Objects)
        {
            utils::draw_line(obj.Origin, obj.EndPoint, 0.02, glm::vec4(0.5, 0.5, 0.5, 1));
        }

        for (Particle& particle : particles)
        {
            glm::vec2 toApply = glm::normalize(particle.ForceDir) * gl::delta_time_in_seconds() * particle.Speed;

            //toApply += particle.Mass * 0.9f * gl::delta_time_in_seconds() * glm::vec2(0, -1);

            //toApply += (gl::mouse_position() - particle.Pos) * gl::delta_time_in_seconds() * (particle.Mass /10);

            Vector2D vec(particle.Pos, particle.ForceDir, particle.Speed);

            for (ObjectData& obj : Objects)
            {
                glm::vec2 dir(obj.EndPoint - obj.Origin);
                Vector2D vecL(obj.Origin, glm::normalize(dir), glm::length(dir));
                if (GetIntersectionPoint(vec, vecL) != glm::vec2(-123.0f, -123.0f)) 
                {
                    toApply = glm::reflect(toApply, glm::vec2(dir.y, dir.x));
                }
            }

            particle.Pos += toApply;

            // if (particle.LifeMore < 0)
            // {
            //     particle.LifeSize -= gl::delta_time_in_seconds() / (particle.LifeSize * 1000);
            //     particle.LifeSize = glm::clamp(particle.LifeSize, 0.0f, 10.0f);
            // }
            // else particle.LifeMore -= gl::delta_time_in_seconds();
            
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