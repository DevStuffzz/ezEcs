#pragma once



namespace EzEcs {
    class Entity; // Forward declaration to avoid circular dependency

    class Component {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual void Initialize() {}
        virtual void Start() {}
        virtual void Update(float deltaTime) {}

        void SetActive(bool active) { isActive = active; }
        bool IsActive() const { return isActive; }

        // Set the entity this component belongs to
        void SetEntity(Entity* entity) { this->entity = entity; }

        // Get the entity this component belongs to
        Entity* GetEntity() const { return entity; }



#ifdef EZ_IMGUI
        // ImGui rendering for a generic component
        virtual void renderImGui() {
            ImGui::Text("Component (ImGui Not implemented): %s", typeid(*this).name());
        }
#endif

    private:
        bool isActive = true;
        Entity* entity = nullptr; // Pointer to the owning entity
    };
}