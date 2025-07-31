#pragma once

class Scene;

class System {
protected:
    Scene* scene = nullptr;

public:
    virtual ~System() = default;

   // Called once when the scene is set up
    virtual void setup() {}

    // Called every frame for logic updates
    virtual void update() {}

    // Called every frame for rendering
    virtual void render() {}

    // Set by the Scene when the system is added
    virtual void setScene(Scene* s) { scene = s; }
};


