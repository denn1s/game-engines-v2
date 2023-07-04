#include "print.h"
#include "Pong.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
    // Constructor implementation
}

Pong::~Pong() {
    // destructor implementation
}

void Pong::setup() {
    print("Setting up Pong game...");
    // Add your Pong-specific setup code here
}
