## OpenGL Graphics Sandbox Engine

This is a graphics sandbox engine written in modern OpenGL 4.5,
it provides a basic abstraction for VAO, VBO, IBO, Textures, Layout, Windowing system etc
with some libraries
and used for testing various graphics / rendering techniques such as -

- Blending
- Camera System
- Per pixel lighting
- Model loading
  - Meshes
  - Materials
  - Textures
- Culling (Frustum, Occlusion)*
- Normal Mapping*
- Cube Maps (Skybox)*
- Scene Graph*
- Deferred Rendering*
- PBR*

*TODO

---
### Screenshots
![SponzaScreenshot](https://user-images.githubusercontent.com/112700146/211896625-0da1e315-d9cb-4a74-b601-f62a1029daec.jpg)

![SponzaVideo](https://user-images.githubusercontent.com/112700146/211896959-34e0a19b-f336-4aec-8c85-71e2d961e6e2.mp4)

---
### Installation

**Prerequisite** - Windows 64 bit, Visual studio 2019 or higher

1. Clone repo

```bash
git clone https://github.com/orbitingotter/OpenGL.git
```

2. Download 3D Models (not included in project because models are freaking huuuge)

    - [Sponza Scene](https://www.markdownguide.org)

3. Extract and place it in OpenGL/Resources/Models
4. Set your default test (recommened ModelTest) in main (App.cpp)
```cpp
Sandbox* scene = new ModelTest();
```
and make sure correct path is loaded in (ModelTest.h)
```cpp
mModel = std::make_unique<Model>("Resources/Models/sponza/sponza.obj");
```

5. Build and run.

---
### Dependencies

- **GLEW** - OpenGL Function Pointers
- **GLFW** - Window, Input polling, OpenGL context
- **glm** - OpenGL Mathematics Library
- **stb_image** - Image Loading
- **imgui** - GUI Library
- **assimp** - Model/Mesh Importer