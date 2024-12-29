# ezEcs
Simple Lightweight Entity Component System in C++

## Pros

* Header Only
* Fast
* Single Header Include

## Usage

```
// #define EZ_IMGUI; define EZ_IMGUI for ImGui Rendering Capabilities. EzEcs does not include ImGui so you will need to import it yourself if you wish to use

#include "EzEcs/EzEcs.h" // Single Header Include

#include <iostream>


// Components Are classes extended from EzEcs::Component
class ExampleComponent : public EzEcs::Component {
public:
	ExampleComponent() = default;
	~ExampleComponent() = default;

	// Called when the scene is now active
	void Start() override {
		std::cout << "Starting Component" << std::endl;
	}

	// called every frame that the scene is active
	void Update(float dt) override {
		if(!updated)
			std::cout << "Updating Component" << std::endl;
		updated = true;
	}

	void ExampleMethod() {
		std::cout << "Example Method" << std::endl;
	}

private:
	bool updated = false;
};

// Component with constructor pass in
class ComponentWithPassIn : public EzEcs::Component {
public:
	ComponentWithPassIn(const char* PassIn) {
		std::cout << PassIn << std::endl;
	}

	// Called when the scene is now active
	void Start() override {
		std::cout << "Starting Component w/ Pass in" << std::endl;
	}

	// called every frame that the scene is active
	void Update(float dt) override {
		if (!updated)
			std::cout << "Updating Component w/ Pass in" << std::endl;
		updated = true;
	}

private:
	bool updated = false;
};

int main() {
	EzEcs::SceneManager& SceneManager = EzEcs::SceneManager::getInstance();
	SceneManager.addScene("Example Scene");

	std::shared_ptr<EzEcs::Scene>& ExampleScene = SceneManager.getScene(0);

	std::shared_ptr<EzEcs::Entity>&  ExampleEntity = ExampleScene->createEntity();
	ExampleEntity->AddComponent<ExampleComponent>();
	ExampleEntity->AddComponent<ComponentWithPassIn>("Example");

	SceneManager.setActiveScene(0);

	bool running = true;

	ExampleEntity->GetComponent<ExampleComponent>()->ExampleMethod();

	while (running) {
		SceneManager.update(0.01f);
	}
}```
