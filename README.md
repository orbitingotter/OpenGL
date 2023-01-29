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
- Cube Maps (Skybox)
- Shadow Mapping
- Normal Mapping*
- Culling (Frustum, Occlusion)*
- Scene Graph*
- Deferred Rendering*
- PBR*

*TODO

---
### Screenshots
![SponzaScreenshot](https://user-images.githubusercontent.com/112700146/215332941-91b1b174-3359-4278-9e50-f30f53f44d75.jpg)

https://user-images.githubusercontent.com/112700146/215334019-6ce7eca4-3d2c-42eb-b0a7-eef1e5db8960.mp4

https://user-images.githubusercontent.com/112700146/215154063-502dbf6d-d4e9-47af-846f-cc3451e8e226.mp4

---
### Installation

**Prerequisite** - Windows 64 bit, Visual studio 2019 or higher

1. Clone repo

```bash
git clone https://github.com/orbitingotter/OpenGL.git
```
2. Download 3D Models (not included in project because models are freaking huuuge)

    - [Sponza Scene](https://casual-effects.com/g3d/data10/index.html#mesh8)

3. Extract and place it in OpenGL/Resources/Models
4. Set your default test (recommened ShadowTest) in main (App.cpp)
```cpp
Sandbox* scene = new ShadowTest();
```
and make sure correct path is loaded
```cpp
mModel = std::make_unique<Model>("Resources/Models/sponza/sponza.obj");
```

5. Build and run.

---
### Controls
*Mouse* - Look Around\
*Shift* - Down\
*Space* - Up\
*C* - Switch between Free Roam and Selection

---
### Dependencies

- **GLEW** - OpenGL Function Pointers
- **GLFW** - Window, Input polling, OpenGL context
- **glm** - OpenGL Mathematics Library
- **stb_image** - Image Loading
- **imgui** - GUI Library
- **assimp** - Model/Mesh Importer
