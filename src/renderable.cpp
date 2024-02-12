#include <renderable.h>

using namespace BEG;

void RenderSystem::updateAll(Game& game, std::vector<std::tuple<Transform&, Renderable&>> view) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto [transform, renderable] : view) {
        renderable.shader.use();

        renderable.shader.setUniform("model", transform.toMatrix());
        renderable.shader.setUniform("combined", game.camera.combinedMatrix(game.aspectRatio(), 0.1f, 100.0f));

        renderable.shader.setUniform("viewPosition", game.camera.position);

        renderable.shader.setUniform("lightable", renderable.lightable);

        if (renderable.lightable) {
            renderable.shader.setUniform("material.ambient", Vector<3>{ renderable.material.ambient.r(), renderable.material.ambient.g(), renderable.material.ambient.b() });
            renderable.shader.setUniform("material.diffuse", Vector<3>{ renderable.material.diffuse.r(), renderable.material.diffuse.g(), renderable.material.diffuse.b() });
            renderable.shader.setUniform("material.specular", Vector<3>{ renderable.material.specular.r(), renderable.material.specular.g(), renderable.material.specular.b() });
            renderable.shader.setUniform("material.shininess", renderable.material.shininess);

            auto directionalLightView{ game.scene.view<DirectionalLight>() };
            renderable.shader.setUniform("numberOfDirectionalLights", static_cast<int>(directionalLightView.size()));

            for (size_t i{ 0 }; i < directionalLightView.size(); ++i) {
                auto [light] = directionalLightView[i];
                
                renderable.shader.setArrayUniform("directionalLights", "direction", i, -light.direction.normalized());
                renderable.shader.setArrayUniform("directionalLights", "color", i, light.color.toVector());
                renderable.shader.setArrayUniform("directionalLights", "ambientStrength", i, light.ambientStrength);
            }

            auto pointLightView{ game.scene.view<Transform, PointLight>() };
            renderable.shader.setUniform("numberOfPointLights", static_cast<int>(pointLightView.size()));

            for (size_t i{ 0 }; i < pointLightView.size(); ++i) {
                auto [transform, light] = pointLightView[i];
                
                renderable.shader.setArrayUniform("pointLights", "position", i, transform.position);
                renderable.shader.setArrayUniform("pointLights", "color", i, light.color.toVector());
                renderable.shader.setArrayUniform("pointLights", "radius", i, light.radius);
                renderable.shader.setArrayUniform("pointLights", "ambientStrength", i, light.ambientStrength);
            }

            auto spotLightView{ game.scene.view<Transform, SpotLight>() };
            renderable.shader.setUniform("numberOfSpotLights", static_cast<int>(spotLightView.size()));
            for (size_t i{ 0 }; i < spotLightView.size(); ++i) {
                auto [transform, light] = spotLightView[i];

                renderable.shader.setArrayUniform("spotLights", "position", i, transform.position);
                renderable.shader.setArrayUniform("spotLights", "direction", i, Vector<3>(transform.orientation.toMatrix() * Vector<4>(0.0f, 0.0f, -1.0f, 1.0f)));
                renderable.shader.setArrayUniform("spotLights", "color", i, light.color.toVector());
                renderable.shader.setArrayUniform("spotLights", "range", i, light.range);
                renderable.shader.setArrayUniform("spotLights", "angle", i, light.angle);
                renderable.shader.setArrayUniform("spotLights", "blurAngle", i, light.blurAngle);
            }
        }

        renderable.model.render();
    }
}