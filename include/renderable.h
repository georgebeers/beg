#ifndef BEG_RENDERABLE_H
#define BEG_RENDERABLE_H

#include <shader.h>
#include <model.h>
#include <ecs.h>
#include <game.h>
#include <basic.h>

namespace BEG {

struct Material {
    Color ambient{ 1.0f, 1.0f, 1.0f };
    Color diffuse{ 1.0f, 1.0f, 1.0f };
    Color specular{ 1.0f, 1.0f, 1.0f };

    float shininess{ 1.0f };
};

struct Light : Component {
    Color color{ 1.0f, 1.0f, 1.0f };
    float ambientStrength{ 0.1f };
};

struct DirectionalLight : Light {
    Vector<3> direction{ 0.0f, -1.0f, 0.0f };
};

struct PointLight : Light {
    float radius{ 1.0f };
};

struct SpotLight : Light {
    float range{ 1.0f }, angle{ M_PIf32 / 2.0f }, blurAngle{ 0.0f };
};

struct Renderable : Component {
    Model model{};
    Material material{};
    Shader shader{};

    bool lightable{ true };
};

class RenderSystem : public System<Transform, Renderable> {
    void updateAll(Game& game, std::vector<std::tuple<Transform&, Renderable&>> view);
};

}

#endif