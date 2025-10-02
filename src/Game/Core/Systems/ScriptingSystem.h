#pragma once

#include "ECS/System.h"
#include <sol/sol.hpp>

class ScriptingSystem : public System {
public:
    ScriptingSystem(const std::string& script_path);
    ~ScriptingSystem() = default;

    void update() override;

private:
    sol::state lua;
    std::string script_path;
    float total_time = 0.0f;
};
