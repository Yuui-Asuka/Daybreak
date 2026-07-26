# 🌅 Daybreak Engine

<p align="center">

<img src="https://img.shields.io/badge/Graphics-Vulkan-red?style=for-the-badge&logo=vulkan">
<img src="https://img.shields.io/badge/Language-C%2B%2B20-blue?style=for-the-badge&logo=cplusplus">
<img src="https://img.shields.io/badge/Build-CMake-green?style=for-the-badge&logo=cmake">
<img src="https://img.shields.io/badge/Platform-Windows-lightgrey?style=for-the-badge&logo=windows">

</p>

---

# 🚀 About Daybreak

**Daybreak** is a custom rendering engine built from scratch with modern C++ and Vulkan.

The goal of this project is to explore the architecture behind modern real-time rendering engines, including:

- Low-level GPU programming
- Vulkan rendering pipeline
- Rendering abstraction design
- Scene management
- Asset pipeline
- Physically Based Rendering

Unlike traditional engines that hide graphics APIs behind high-level abstractions, Daybreak starts from the foundation and builds the entire rendering stack step by step.

```
Application
     |
     |
  Renderer
     |
     |
 Vulkan Backend
     |
     |
    GPU
```

---

# ✨ Features

## ✅ Completed

### Vulkan Rendering Core

- Vulkan Instance
- Physical Device Selection
- Logical Device
- Surface Creation
- Swapchain
- Render Pass
- Framebuffer
- Graphics Pipeline
- Command Pool
- Command Buffer
- Synchronization System


### Mesh Rendering

- Vertex structure
- Vertex Buffer
- Index Buffer
- Mesh abstraction
- Indexed Drawing


Rendering flow:

```
CPU Mesh Data

      |
      v

Vertex Buffer
Index Buffer

      |
      v

vkCmdDrawIndexed()

      |
      v

GPU Rendering
```

---

# 🏗️ Architecture

Current project structure:

```
Daybreak/

├── Assets
│
│   ├── Shaders
│   ├── Models
│   └── Textures
│

├── Engine
│
│   ├── Core
│   │
│   │   ├── Application
│   │   └── Engine
│   │
│   ├── Renderer
│   │
│   │   ├── Mesh
│   │   ├── Buffer
│   │   ├── Camera
│   │   ├── Texture
│   │   │
│   │   └── Vulkan
│   │
│   ├── Scene
│   │
│   ├── Math
│   │
│   └── Resource
│
├── Sandbox
│
├── ThirdParty
│
└── CMakeLists.txt
```

---

# 🎮 Rendering Pipeline

Current rendering pipeline:

```
Application

    |
    v

Mesh

    |
    v

VulkanBuffer

    |
    v

CommandBuffer

    |
    v

Graphics Pipeline

    |
    v

GPU
```

---

# 🧩 Roadmap

## Rendering

- [x] Triangle Rendering
- [x] Vertex Buffer
- [x] Index Buffer
- [ ] Uniform Buffer (MVP)
- [ ] Descriptor Set
- [ ] Texture Sampling
- [ ] Depth Testing
- [ ] Shadow Mapping


## Camera System

- [ ] Transform System
- [ ] Perspective Camera
- [ ] FPS Camera Controller
- [ ] Mouse Input
- [ ] Scroll Zoom


## Asset Pipeline

- [ ] OBJ Loader
- [ ] glTF Loader
- [ ] Material System
- [ ] Resource Manager


## Lighting

- [ ] Blinn-Phong Lighting
- [ ] Normal Mapping
- [ ] HDR Rendering
- [ ] Physically Based Rendering (PBR)


## Engine Architecture

- [ ] Entity Component System
- [ ] Scene Serialization
- [ ] Editor
- [ ] Runtime Layer
- [ ] Multi-threaded Rendering

---

# 🔧 Technology Stack

| Component | Technology |
|---|---|
| Language | C++20 |
| Graphics API | Vulkan |
| Window System | GLFW |
| Math Library | GLM |
| Build System | CMake |
| Package Manager | vcpkg |
| Shader Language | GLSL |

---

# 🖥️ Current Development Stage

Daybreak is currently in the **Vulkan Rendering Foundation Stage**.

Current achievement:

```
CPU Data

    |
    v

GPU Buffer

    |
    v

Vulkan Pipeline

    |
    v

Triangle Rendering
```

The next milestone:

```
Mesh

 |

Material

 |

Texture

 |

Camera

 |

Lighting

 |

PBR

 |

Scene

 |

Editor
```

---

# 📌 Philosophy

Daybreak follows one simple principle:

> Understand every layer between code and pixels.

From:

```
C++ Object
```

to:

```
GPU Command
```

to:

```
Final Pixel
```

Every system is built step by step to understand how modern graphics engines work internally.

---

# 📷 Screenshots

Coming soon.

---

# 📄 License

This project is currently under development.