#include <beg.h>

#include <iostream>

struct SpinnerComponent : BEG::Component {
    float speed{ 0.0f };
};

struct MoverComponent : BEG::Component {
    float movementSpeed{ 3.0f };
    float rotationSpeed{ 0.01f };

    BEG::Vector<3> orientation{ 0.0f };

    float mouseX{}, mouseY{};
};

class SpinSystem : public BEG::System<BEG::Transform, SpinnerComponent> {
    void update(BEG::Game& game, BEG::Transform& transform, SpinnerComponent& spinner) {
        transform.orientation *= BEG::Quaternion(spinner.speed * game.deltaTime(), spinner.speed * game.deltaTime(), 0.0f);
    }
};

class MovementSystem : public BEG::System<MoverComponent> {
    void update(BEG::Game&game, MoverComponent& mover) {        
        if (game.isKeyDown(BEG::Keyboard::Key::W)) {
            game.camera.position += game.camera.front() * (game.deltaTime() * mover.movementSpeed);
        } else if (game.isKeyDown(BEG::Keyboard::Key::S)) {
            game.camera.position += game.camera.front() * (game.deltaTime() * -mover.movementSpeed);
        }
        if (game.isKeyDown(BEG::Keyboard::Key::A)) {
            game.camera.position += game.camera.right() * (game.deltaTime() * -mover.movementSpeed);
        } else if (game.isKeyDown(BEG::Keyboard::Key::D)) {
            game.camera.position += game.camera.right() * (game.deltaTime() * mover.movementSpeed);
        }

        float dMX{ mover.mouseX - game.mouseX() };
        float dMY{ mover.mouseY - game.mouseY() };

        mover.mouseX = game.mouseX();
        mover.mouseY = game.mouseY();

        float max = (BEG::Pi / 2.0f) - 0.1f;

        mover.orientation.x(BEG::clamp(mover.orientation.x() + dMY * mover.rotationSpeed, -max, max));
        mover.orientation.y(mover.orientation.y() + dMX * mover.rotationSpeed);

        std::cout << dMX << ",  " << dMY << '\n';

        game.camera.orientation = BEG::Quaternion(mover.orientation);
    }
};

int main() {
    BEG::Game game{ "BEG", 800, 800 };

    game.camera.position = BEG::Vector<3>(0.0f, 0.0f, 5.0f);

    BEG::Entity cube{ game.scene.newEntity() };

    game.scene.addComponent<SpinnerComponent>(cube);
    game.scene.addComponent<MoverComponent>(cube);

    game.scene.addComponent<BEG::Transform>(cube);
    BEG::Renderable& cubeRend{ game.scene.addComponent<BEG::Renderable>(cube) };

    cubeRend.model = BEG::Model::cube({
        0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2,
        3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5,
    }, {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 1.0f }
    });

    cubeRend.material = {
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        2.0f
    };

    try {
        cubeRend.shader = BEG::Shader::fromPaths("./resources/shaders/basic.vert", "./resources/shaders/basic.frag");
    } catch (BEG::Shader::ShaderError err) {
        std::cout << err.info << '\n';
        throw err;
    }

    BEG::Entity plane{ game.scene.newEntity() };
    BEG::Transform& planeTransform{ game.scene.addComponent<BEG::Transform>(plane) };
    planeTransform.position = BEG::Vector<3>(0.0f, -1.0f, 0.0f);
    planeTransform.scale = BEG::Vector<3>(100.0f, 1.0f, 100.0f);

    BEG::Renderable& planeRend{ game.scene.addComponent<BEG::Renderable>(plane) };

    planeRend.model = BEG::Model({
        { -1.0f, 0.0f, -1.0f },
        {  1.0f, 0.0f, -1.0f },
        {  1.0f, 0.0f,  1.0f },
        {  1.0f, 0.0f,  1.0f },
        { -1.0f, 0.0f,  1.0f },
        { -1.0f, 0.0f, -1.0f },    
    }, {
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f }
    }, {
        0, 0, 0, 0, 0, 0
    }, {
        { 1.0f, 1.0f, 1.0f }
    });
    planeRend.material.specular = BEG::Color(0.1f, 0.1f, 0.1f);

    planeRend.shader = BEG::Shader::fromPaths("./resources/shaders/basic.vert", "./resources/shaders/basic.frag");

    BEG::Entity directional{ game.scene.newEntity() };

    BEG::DirectionalLight& directionalProperties{ game.scene.addComponent<BEG::DirectionalLight>(directional) };

    directionalProperties.color = BEG::Color(0.2f, 0.2f, 0.2f);
    directionalProperties.direction = BEG::Vector<3>(0.5f, -1.0f, 0.5f);
    
    BEG::Entity point{ game.scene.newEntity() };

    BEG::Transform& pointTransform{ game.scene.addComponent<BEG::Transform>(point) };

    pointTransform.position = BEG::Vector<3>(2.0f, 2.0f, -2.0f);
    pointTransform.scale = BEG::Vector<3>(0.1f, 0.1f, 0.1f);

    BEG::PointLight& pointProperties{ game.scene.addComponent<BEG::PointLight>(point) };

    pointProperties.color = BEG::Color(1.0f, 0.0f, 0.0f);
    pointProperties.radius = 5.0f;
    
    BEG::Renderable& pointRend{ game.scene.addComponent<BEG::Renderable>(point) };

    pointRend.lightable = false;
    pointRend.model = BEG::Model::cube(pointProperties.color);
    pointRend.shader = BEG::Shader::fromPaths("./resources/shaders/basic.vert", "./resources/shaders/basic.frag");

    BEG::Entity spot{ game.scene.newEntity() };

    BEG::Transform& spotTransform{ game.scene.addComponent<BEG::Transform>(spot) };

    spotTransform.position = BEG::Vector<3>(-3.0f, 5.0f, -4.0f);
    spotTransform.orientation = BEG::Quaternion(BEG::toRadians(-110.0f), 0.0f, BEG::toRadians(20.0f));
    spotTransform.scale = BEG::Vector<3>(0.1f, 0.1f, 0.1f);

    BEG::SpotLight& spotProperties{ game.scene.addComponent<BEG::SpotLight>(spot) };

    spotProperties.color = BEG::Color(0.0f, 0.0f, 1.0f);
    spotProperties.range = 5.0f;

    spotProperties.angle = BEG::toRadians(30.0f);
    spotProperties.blurAngle = BEG::toRadians(5.0f);

    BEG::Renderable& spotRend{ game.scene.addComponent<BEG::Renderable>(spot) };

    spotRend.lightable = false;
    spotRend.model = BEG::Model::cube(spotProperties.color);
    spotRend.shader = BEG::Shader::fromPaths("./resources/shaders/basic.vert", "./resources/shaders/basic.frag");

    game.scene.registerSystem<SpinSystem>();
    game.scene.registerSystem<MovementSystem>();
    game.scene.registerSystem<BEG::RenderSystem>();

    game.begin();

    return 0;
}