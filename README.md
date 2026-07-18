# Daybreak

# 🌅 Daybreak Engine

> **A Modern Vulkan Rendering Engine built from scratch in C++.**

Daybreak is a learning-oriented real-time rendering engine focused on understanding modern graphics programming from the ground up.

Instead of relying on existing engines, every major rendering system is implemented manually using **Vulkan**, providing complete control over the rendering pipeline and GPU resources.

---

## ✨ Features

### Rendering

* Vulkan Renderer
* Vulkan Instance & Surface
* Physical / Logical Device Selection
* Swapchain Management
* Render Pass
* Graphics Pipeline
* Framebuffer Management
* Command Pool & Command Buffers
* Synchronization (Fence / Semaphore)
* Vertex Buffer Rendering

### Mesh

* Custom Vertex Structure
* Mesh Class
* CPU → GPU Vertex Upload
* Triangle Rendering from CPU Data

### Window System

* GLFW Window
* Resize Detection
* Swapchain Recreation

### Architecture

* Modular Renderer Design
* RAII-style Vulkan Wrappers
* Object-Oriented Engine Structure

---

# Project Structure

```text
Daybreak/
│
├── Assets/
│   └── Shaders/
│
├── Engine/
│   ├── Core/
│   ├── Mesh/
│   ├── Renderer/
│   │   └── Vulkan/
│   └── Window/
│
├── Sandbox/
│
├── CMakeLists.txt
└── README.md
```

---

# Rendering Pipeline

```text
Application
      │
      ▼
Vulkan Context
      │
      ▼
Physical Device
      │
      ▼
Logical Device
      │
      ▼
Swapchain
      │
      ▼
RenderPass
      │
      ▼
Graphics Pipeline
      │
      ▼
Framebuffer
      │
      ▼
Command Buffer
      │
      ▼
Graphics Queue
      │
      ▼
Present Queue
```

---

# Current Progress

* [x] Vulkan Initialization
* [x] Device Selection
* [x] Swapchain
* [x] Render Pass
* [x] Graphics Pipeline
* [x] Shader Modules
* [x] Framebuffers
* [x] Command Buffers
* [x] Synchronization
* [x] Vertex Buffer
* [x] Mesh System
* [x] Window Resize Handling

---

# Roadmap

## Rendering

* [ ] Index Buffer
* [ ] Uniform Buffer
* [ ] Descriptor Sets
* [ ] Texture Loading
* [ ] Sampler
* [ ] Push Constants
* [ ] Dynamic Rendering

## Scene

* [ ] Transform Component
* [ ] Camera
* [ ] Scene Graph
* [ ] Entity System

## Assets

* [ ] OBJ Loader
* [ ] glTF Loader
* [ ] Material System
* [ ] Texture Manager

## Lighting

* [ ] Directional Light
* [ ] Point Light
* [ ] Shadow Mapping
* [ ] PBR Rendering

## Engine

* [ ] Resource Manager
* [ ] Renderer Abstraction
* [ ] ImGui Integration
* [ ] Multi-Frame Rendering
* [ ] Render Graph

---

# Technologies

* **C++20**
* **Vulkan 1.3**
* **GLFW**
* **GLM**
* **CMake**
* **Vcpkg**

---

# Philosophy

Daybreak is built with one goal:

> **Understand every layer of a modern rendering engine instead of hiding it.**

Every Vulkan object is wrapped manually, every rendering stage is implemented explicitly, and every subsystem is designed to be readable, extensible, and educational.

---

# Screenshot

> *(Coming Soon)*

---

# License

This project is released under the **MIT License**.

---

<p align="center">
    <strong>🌅 Daybreak Engine</strong><br>
    Learn • Build • Render
</p>
