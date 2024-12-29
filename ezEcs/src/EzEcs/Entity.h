#pragma once

#include "vector"
#include "Component.h"

namespace EzEcs {
 
    class Entity {
    public:
        // Constructor
        Entity() = default;

        // Destructor
        virtual ~Entity() = default;

        template <typename T, typename... Args>
        T* AddComponent(Args&&... args) {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

            // Create a unique_ptr for the new component
            auto component = std::make_unique<T>(std::forward<Args>(args)...);

            // Set this entity as the owner of the component
            component->SetEntity(this);

            // Get a raw pointer to the component
            T* rawPointer = component.get();

            // Move the unique_ptr into the vector
            components.push_back(std::move(component));

            // Return the raw pointer
            return rawPointer;
        }

        // Get a component of a specific type
        template <typename T>
        T* GetComponent() {
            for (const auto& component : components) {
                if (T* derived = dynamic_cast<T*>(component.get())) {
                    return derived;
                }
            }
            return nullptr;
        }

        // Update all components
        void Update(float deltaTime) {
            for (auto& component : components) {
                if (component->IsActive()) {
                    component->Update(deltaTime);
                }
            }
        }

        // Initialize all components
        void Initialize() {
            for (auto& component : components) {
                component->Initialize();
            }
        }

        void Start() {
            for (auto& component : components) {
                component->Start();
            }
        }

#ifdef EZ_IMGUI
        void renderImGui() {
            if (ImGui::TreeNode((name + "##Entity").c_str())) {
                ImGui::Text("Name: %s", name.c_str());
                ImGui::Text("Components: %zu", components.size());

                // Render each component
                for (size_t i = 0; i < components.size(); ++i) {
                    auto& component = components[i];
                    if (ImGui::TreeNode((std::string("Component ") + std::to_string(i)).c_str())) {
                        component->renderImGui();
                        ImGui::TreePop();
                    }
                }

                ImGui::TreePop();
            }
        }
#endif
    private:
        std::vector<std::unique_ptr<Component>> components; // Store all components as unique_ptr
    };
}