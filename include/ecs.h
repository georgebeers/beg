#ifndef BEG_ECS_H
#define BEG_ECS_H

#include <algorithm>
#include <vector>
#include <memory>
#include <tuple>
#include <optional>

namespace BEG {

using Entity = unsigned long;

class Game;
class Scene;

class Component {
private:
    Entity mOwner{};
public:
    virtual ~Component() = default;

    friend Scene;
};

class SystemInterface {
public:
    virtual ~SystemInterface() = default;

    virtual bool appliesTo(Scene&, Entity) { return false; }

    virtual void callSetup(Game&, Scene&, Entity) {}
    virtual void callUpdate(Game&, Scene&, Entity) {}
    virtual void callDestroy(Scene&, Entity) {}

    virtual void callSetupAll(Game&, Scene&) {}
    virtual void callUpdateAll(Game&, Scene&) {}
};

class Scene {
private:
    Entity mEntityCounter{};

    std::vector<std::shared_ptr<Component>> mComponents{};
    std::vector<Entity> mEntities{};

    std::vector<std::shared_ptr<SystemInterface>> mSystems{};
public:
    enum class SceneError {
        ComponentDoesNotExistError
    };

    Scene() : mEntityCounter{ 0 }, mComponents{}, mEntities{}, mSystems{} {}

    Entity newEntity();

    template <typename T>
    T& addComponent(Entity entity) {
        mComponents.push_back(std::make_shared<T>());
        (*mComponents.back()).mOwner = entity;
        return *std::dynamic_pointer_cast<T>(mComponents.back());
    }

    void deleteEntity(Entity entity);

    bool exists(Entity entity);

    template <typename T>
    bool hasComponent(Entity entity) {
        for (auto component : mComponents) {
            if ((*component).mOwner == entity && std::dynamic_pointer_cast<T>(component) != nullptr) {
                return true;
            }
        }

        return false;
    }

    template <typename... Ts>
    bool hasComponents(Entity entity) {
        return ( hasComponent<Ts>(entity) && ... );        
    }

    template <typename T>
    T& getComponent(Entity entity) {
        for (auto component : mComponents) {
            if ((*component).mOwner == entity && std::dynamic_pointer_cast<T>(component) != nullptr) {
                return *std::dynamic_pointer_cast<T>(component);
            }
        }

        throw SceneError::ComponentDoesNotExistError;
    }

    template <typename... Ts>
    std::tuple<Ts&...> getComponents(Entity entity) {
        return std::tie(getComponent<Ts>(entity)...);
    }

    template <typename... Ts>
    std::vector<std::tuple<Ts&...>> view() {
        std::vector<std::tuple<Ts&...>> stack{};
        for (auto entity : mEntities) {
            if (!hasComponents<Ts...>(entity)) {
                continue;
            }

            stack.push_back(getComponents<Ts...>(entity));
        }

        return stack;
    }

    template <typename T>
    void registerSystem() {
        mSystems.push_back(std::make_shared<T>());
    }

    void setupSystems(Game& game);
    void updateSystems(Game& game);
};

template <typename... Ts>
class System : public SystemInterface {
public:
    virtual ~System() = default;

    virtual void setup(Game&, Ts&...) {}
    virtual void update(Game&, Ts&...) {}
    virtual void destroy(Ts&...) {}

    virtual void setupAll(Game&, std::vector<std::tuple<Ts&...>>) {}
    virtual void updateAll(Game&, std::vector<std::tuple<Ts&...>>) {}

    bool appliesTo(Scene& scene, Entity entity) {
        return scene.hasComponents<Ts...>(entity);
    }

    void callSetup(Game& game, Scene& scene, Entity entity) {
        setup(game, scene.getComponent<Ts>(entity)...);
    }

    void callUpdate(Game& game, Scene& scene, Entity entity) {
        update(game, scene.getComponent<Ts>(entity)...);
    }

    void callDestroy(Scene& scene, Entity entity) {
        destroy(scene.getComponent<Ts>(entity)...);
    }

    void callSetupAll(Game& game, Scene& scene) {
        setupAll(game, scene.view<Ts...>());
    }

    void callUpdateAll(Game& game, Scene& scene) {
        updateAll(game, scene.view<Ts...>());
    }
};

}

#endif