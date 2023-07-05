#include "print.h"
#include "Systems.h"
#include "Components.h"

HelloSystem::HelloSystem() {
    std::cout << "Hello System Constructor" << std::endl;
}

HelloSystem::~HelloSystem() {
    std::cout << "Hello System Destructor" << std::endl;
}

void HelloSystem::run() {
    std::cout << "Hello System!" << std::endl;
}
