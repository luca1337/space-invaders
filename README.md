# Space Invaders

A modern clone of **Space Invaders** written in **C++**, powered by a custom-built game engine inspired by Unity's architecture.  
Includes a component-based object system, OpenGL rendering, input handling, basic physics, and a full working demo game.

---

## ✨ Features

- 🧱 Custom engine architecture (GameObject + Component model, non-ECS)
- 🎨 2D rendering with OpenGL
- 🎮 Input system for keyboard/mouse controls
- 🧠 Basic physics & collision detection
- 🗂 Resource management (textures, shaders)
- 🔧 Organized multi-project Visual Studio solution
- 📦 Includes both engine (`RenderCore`) and game (`SpaceInvaders`) in one repo

---

## 📁 Project Structure

```
/RenderCore        -> Game engine (rendering, input, components, physics, etc.)
/SpaceInvaders     -> Game built using the engine
/vendor            -> Third-party libraries (e.g., stb_image, glad)
/bin               -> Output binaries (ignored by Git)
/build             -> Intermediate build files (ignored by Git)
RenderCore.sln     -> Visual Studio solution file
```

---

## 🚀 Getting Started

### Requirements

- Visual Studio 2022 or later
- C++20 support
- OpenGL-compatible GPU

### Clone the Repository

```bash
git clone --recurse-submodules https://github.com/yourusername/space-invaders.git
```

### Build & Run (Visual Studio)

1. Open `RenderCore.sln`
2. Set `SpaceInvaders` as the startup project
3. Select configuration (e.g. `Debug | x64`)
4. Build and Run

---

## 🎮 About the Game

The demo game is a faithful reinterpretation of **Space Invaders**, featuring:

- Player movement and shooting
- Enemy waves with formation logic
- Collision detection and game state management
- Sprite-based rendering with OpenGL
- Modular and reusable engine foundation

---

## 🔧 Engine Overview

The engine (`RenderCore`) is structured in a Unity-style architecture, including:

- `Actor` and `Component` system
- `Transform`, `SpriteRenderer`, `Collider`, etc.
- Central `World` manager
- Input and window handling (via platform layer)
- Lightweight logging and utilities
- Shader and texture management

The engine is designed to be easily extended for other 2D/3D games.

---

## 🧪 Dependencies

- [GLAD](https://github.com/Dav1dde/glad) – OpenGL loader
- [stb_image](https://github.com/nothings/stb) – image loading
- [GLM](https://github.com/g-truc/glm) – math library (optional)

All dependencies are included as submodules or source in `vendor/`.

---

## 📸 Screenshots

*(Add screenshots of your game if available)*

---

## 📜 License

This project is licensed under the MIT License – see [`LICENSE`](LICENSE) for details.

---

## 🧠 Credits

Inspired by the original **Space Invaders** arcade game (1978).  
Built for educational, learning, and fun purposes.

---

## 🛠 Future Plans

- 💥 Add sound support
- 🌈 Implement animations
- 🕹 Gamepad input support
- 🧱 Extend engine with scene serialization and scripting

---

Made with ❤️ in C++
