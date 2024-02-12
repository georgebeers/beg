#include <ecs.h>

using namespace BEG;

Entity Scene::newEntity() {
    mEntities.push_back(mEntityCounter++);
    return mEntityCounter - 1;
}

void Scene::deleteEntity(Entity entity) {
    for (auto system : mSystems) {
        if ((*system).appliesTo(*this, entity)) {
            (*system).callDestroy(*this, entity);
        }
    }

    mComponents.erase(std::remove_if(mComponents.begin(), mComponents.end(), [entity](std::shared_ptr<Component> component) {
        return (*component).mOwner == entity;
    }), mComponents.end());
    mEntities.erase(std::remove(mEntities.begin(), mEntities.end(), entity), mEntities.end());
}

bool Scene::exists(Entity entity) {
    for (auto e : mEntities) {
        if (e == entity) {
            return true;
        }
    }

    return false;
}

void Scene::setupSystems(Game& game) {
    for (auto system : mSystems) {
        (*system).callSetupAll(game, *this);
        for (auto entity : mEntities) {
            if ((*system).appliesTo(*this, entity)) {
                (*system).callSetup(game, *this, entity);
            }
        }
    }
}

void Scene::updateSystems(Game& game) {
    for (auto system : mSystems) {
        (*system).callUpdateAll(game, *this);
        for (auto entity : mEntities) {
            if ((*system).appliesTo(*this, entity)) {
                (*system).callUpdate(game, *this, entity);
            }
        }
    }
}