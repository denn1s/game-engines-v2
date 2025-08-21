This is a codebase for computer science students to learn about game engines and entity component systems. It is currently being rewritten to use Raylib instead of SDL+glm, with a focus on simplifying the logic for easy understanding.

When contributing to this codebase, please prioritize code legibility over best practices. For example, raw pointers may be preferred over smart pointers if they make the code easier for students to understand. The goal is to create a clear and accessible reference for learning.

Key characteristics:
- Language: C++
- Library: Raylib
- Architecture: Entity Component System (ECS)
- Focus: Educational, code legibility

When making changes, please adhere to the following guidelines:
- **Simplicity:** Keep the code as simple as possible. Avoid complex language features or design patterns that may be confusing for beginners.
- **Clarity:** Write clear and concise code. Use meaningful variable names and add comments where necessary to explain complex logic.
- **Consistency:** Follow the existing code style and conventions.
- **Raylib Focus:** Ensure that all new features and modifications are implemented using Raylib.

The project is structured as follows:
- `src/`: Contains the source code for the game engine and the game itself.
- `src/ECS/`: The core Entity Component System.
- `src/Game/`: The main game logic.
- `assets/`: Game assets such as sprites and tiles.

Current learning modules are introduced via branches, each branch adding a new feature to the engine. The `main` branch contains the most up-to-date version of the engine.

Remember, the primary audience for this codebase is students. Your contributions should reflect that by being as clear and educational as possible.
