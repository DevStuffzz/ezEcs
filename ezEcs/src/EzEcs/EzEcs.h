#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include "Entity.h"

#ifdef EZ_IMGUI
#include <imgui.h>
#endif

#define uint unsigned int

namespace EzEcs {
    class Scene {
    public:
        Scene(uint id, std::string name)
            : id(id), name(std::move(name)) {}
        ~Scene() = default;

        // Getters
        uint getId() const { return id; }
        const std::string& getName() const { return name; }

        // Scene lifecycle methods
        void init() {
            for (const auto& entity : entities) {
                entity->Initialize();
            }
        }

        void start() {
            for (const auto& entity : entities) {
                entity->Start();
            }
        }

        void update(float deltaTime) {
            for (const auto& entity : entities) {
                entity->Update(deltaTime);
            }
        }

#ifdef EZ_IMGUI
        void renderImGui() {
            if (ImGui::TreeNode((name + "##Scene").c_str())) {
                ImGui::Text("ID: %u", id);
                ImGui::Text("Entities: %zu", entities.size());

                for (size_t i = 0; i < entities.size(); ++i) {
                    auto& entity = entities[i];
                    if (ImGui::TreeNode((std::string("Entity ") + std::to_string(i)).c_str())) {
                        entity->renderImGui(); // Ensure entities support ImGui rendering
                        ImGui::TreePop();
                    }
                }

                ImGui::TreePop();
            }
        }
#endif

        // Add an entity to the scene
        template <typename... Args>
        std::shared_ptr<Entity> createEntity(Args&&... args) {
            auto entity = std::make_shared<Entity>(std::forward<Args>(args)...);
            entities.push_back(entity);
            return entity;
        }

        // Remove an entity from the scene
        void removeEntity(const std::shared_ptr<Entity>& entity) {
            entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
        }

        // Get all entities
        const std::vector<std::shared_ptr<Entity>>& getEntities() const {
            return entities;
        }

    private:
        uint id;                                // Unique ID for the scene
        std::string name;                       // Name of the scene
        std::vector<std::shared_ptr<Entity>> entities; // List of entities in the scene
    };






    class SceneManager {
    public:
        // Singleton pattern to ensure only one SceneManager exists
        static SceneManager& getInstance() {
            static SceneManager instance;
            return instance;
        }

        // Add a scene to the manager, auto-assigning an ID
        void addScene(const std::string& name) {
            uint newId = scenes.size(); // Assign ID based on the size of the map
            scenes[newId] = std::make_shared<Scene>(newId, name);
            scenes[newId]->init();
        }

        // Set the active scene by ID
        void setActiveScene(uint id) {
            if (scenes.find(id) == scenes.end()) {
                throw std::runtime_error("Scene with this ID does not exist.");
            }
            activeScene = scenes[id];
            activeScene->start();
        }

        // Get the active scene
        std::shared_ptr<Scene> getActiveScene() const {
            if (!activeScene) {
                throw std::runtime_error("No active scene is set.");
            }
            return activeScene;
        }


#ifdef EZ_IMGUI
        void renderImGui() {
            if (ImGui::Begin("Scene Manager")) {
                ImGui::Text("Scenes: %zu", scenes.size());

                for (const auto& [id, scene] : scenes) {
                    if (ImGui::TreeNode((scene->getName() + "##" + std::to_string(id)).c_str())) {
                        scene->renderImGui();
                        ImGui::TreePop();
                    }
                }

                if (activeScene) {
                    ImGui::Separator();
                    ImGui::Text("Active Scene:");
                    activeScene->renderImGui();
                }

                ImGui::End();
            }
        }
#endif

        // Get a scene by ID
        std::shared_ptr<Scene> getScene(uint id) const {
            auto it = scenes.find(id);
            if (it == scenes.end()) {
                throw std::runtime_error("Scene with this ID does not exist.");
            }
            return it->second;
        }

        void update(float dt) {
            activeScene->update(dt);
        }

    private:
        // Private constructor for Singleton pattern
        SceneManager() = default;

        // Disable copy and assignment
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;

        std::unordered_map<uint, std::shared_ptr<Scene>> scenes;
        std::shared_ptr<Scene> activeScene = nullptr;
    };
}