# Simple ECS Game Engine

This project is a simple game engine that uses the Entity-Component-System (ECS) architecture. It is intended as a teaching tool for a game engine architecture course. The goal of this project is to provide a clear and easy-to-understand implementation of an ECS, prioritizing clarity for students over advanced features or optimizations.

## Project Structure

The project is organized into the following directories:

*   `src/ECS`: This directory contains the core ECS implementation.
    *   `Components.h`: Defines the basic components used by the engine.
    *   `Entity.h`: Provides an abstraction for entities in the game world.
    *   `System.h`: Defines the base class for all systems.
*   `src/Game`: This directory contains the core game loop and scene management.
    *   `Game.h` and `Game.cpp`: Implement the main game loop.
    *   `Scene.h` and `Scene.cpp`: Manage the entities and systems in a scene.
*   `src/Pong`: This directory contains the implementation of a simple Pong game.
    *   `Components.h`: Defines the components specific to the Pong game.
    *   `Pong.h` and `Pong.cpp`: Set up the Pong game scene.
    *   `Systems.cpp`: Implements the systems that control the Pong game.
*   `scripts`: This directory contains the build and run scripts.
    *   `build.sh`: Compiles the project.
    *   `configure.sh`: Configures the project using CMake.
    *   `run.sh`: Builds and runs the project.
    *   `clean.sh`: Removes the build artifacts.

## Building and Running

This project uses CMake to generate the build files. The following instructions assume you are on a Linux system with `gcc`, `g++`, and `cmake` installed.

### 1. Configure the Project

First, you need to configure the project using CMake. This will generate the necessary build files in the `build` directory.

```bash
./configure.sh
```

### 2. Build the Project

Next, you can build the project using the `build.sh` script. This will compile the code and create an executable in the `build` directory.

```bash
./build.sh
```

### 3. Run the Game

Finally, you can run the game using the `run.sh` script. This will build the project (if necessary) and then run the game.

```bash
./run.sh
```

## Understanding the Code

The main goal of this project is to teach the basics of the ECS architecture. Here are some key concepts to keep in mind as you explore the code:

*   **Entities:** Entities are lightweight objects that represent things in your game world. In this project, entities are just simple IDs managed by the `entt` library.
*   **Components:** Components are simple data structures that hold the data for your entities. For example, the `TransformComponent` holds the position of an entity.
*   **Systems:** Systems are where the logic of your game lives. Each system is responsible for updating a specific aspect of the game. For example, the `MovementSystem` is responsible for updating the position of entities based on their velocity.

By studying the code in this project, you will gain a better understanding of how to use the ECS architecture to create a simple game engine.
