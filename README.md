# ImGui Integration Plan

This document outlines the plan to integrate ImGui into the game engine using rlImGui.

## 1. Project Setup

- **Download rlImGui:** Download the rlImGui library from its GitHub repository.
- **Add to Project:** Add the rlImGui source files to the project's `src` directory, under a new `ImGui` directory.
- **Update CMakeLists.txt:** Update the `CMakeLists.txt` file to include the ImGui and rlImGui source files in the build.

## 2. ImGui System

A single `ImGuiSystem` will be created to handle all ImGui-related logic. This system will have the following responsibilities:

- **Setup:**
    - Initialize ImGui and rlImGui in the `setup()` method. This will be called once when the scene is loaded.
- **Render:**
    - Handle the rendering of the ImGui UI in the `render()` method. This will be called every frame.
    - The `render()` method will contain the following logic:
        - Start a new ImGui frame.
        - Create a dropdown menu that lists all entities in the scene.
        - When an entity is selected from the dropdown, display its components and their properties.
        - End the ImGui frame and render the UI.

## 3. Component Serialization

To display the properties of a component, a simple serialization mechanism will be implemented. Each component will have a `toString()` method that returns a string representation of its properties. This string will then be displayed in the ImGui UI.

## 4. Game Loop Integration

- **Create ImGuiSystem:** An instance of the `ImGuiSystem` will be created in the `Pong.cpp` file.
- **Add to Scene:** The `ImGuiSystem` will be added to the scene using the `addSystem()` method.
- **Render:** The `render()` method of the `ImGuiSystem` will be called from the `Game::render()` method.

## 5. Implementation Steps

1. **Download and integrate rlImGui.**
2. **Create the `ImGuiSystem` class.**
3. **Implement the `setup()` method to initialize ImGui.**
4. **Implement the `render()` method to draw the UI.**
5. **Add a `toString()` method to each component.**
6. **Integrate the `ImGuiSystem` into the game loop.**