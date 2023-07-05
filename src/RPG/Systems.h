#pragma once

#include <iostream>
#include "Engine/ECS/System.h"

class HelloSystem : public SetupSystem {
  public:
    HelloSystem();
    ~HelloSystem();

    void run() override;
};
