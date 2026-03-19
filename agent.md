# StarSystems - Agent Guide

## Project Overview

StarSystems is a 3D OpenGL-based engine for rendering procedurally generated star systems with terrain, water, sky, and atmospheric effects. Written in C++11, it features a CDLOD (Continuous Distance-based Level of Detail) terrain system, Perlin noise generation, instanced rendering, and multithreaded asset loading.

## Build System

**CMake 3.0+**, compiles to static library `libengine.a` and executable `stars`.

```bash
mkdir build && cd build
cmake ..
make
```

**Targets:**
- `stars` - main executable
- `starsystems_test` - unit tests (Google Test)
- `format` - run clang-format on all project sources
- `cppcheck` - static analysis

**Run tests:**
```bash
cd build && ctest
```

**System dependencies:** `build-essential libxmu-dev libxi-dev libgl-dev`, GLFW 3.2+, OpenGL 4.0+

**Vendored deps (in `deps/`):** GLEW 2.2.0, Assimp 5.0.1, GLM, stb_image, Google Test

## Project Structure

```
src/starsystems.cpp          # Entry point - creates Engine, Scene, terrain, starts render loop
lib/engine.cpp               # Engine class: window init (GLFW/OpenGL 4.3), thread pool, render loop
lib/drawable/                # Renderable objects: Drawable base, Mesh (VAO/VBO/EBO), DrawableFactory, primitives
lib/render/                  # Rendering pipeline: Renderer, Shader base + 9 implementations, specialized renderers
lib/scene/                   # Scene management: Scene, View (camera/frustum), Gui
lib/terrain/                 # Terrain system: TerrainManager, CDLOD tree, TerrainGenerator, Perlin noise, heightmaps
lib/utils/                   # Utilities: ThreadPool, TextureLoader, AssetLoader, math utils
include/                     # Public headers (mirrors lib/ structure)
include/global.hpp           # Extern globals: g_deltaTime, g_currentFrameTime, g_triangleCount, g_threadPool
include/basetypes.hpp        # ShaderType enum, Texture struct
shader/                      # GLSL shaders: .vs, .fs, .gs, .tcs, .tes (30+ files)
assets/                      # Textures, models (symlinked into build dir)
test/library/                # Unit tests: drawableTest.cpp, mathutilsTest.cpp
deps/                        # Vendored dependencies
```

## Architecture

### Rendering Pipeline

```
Engine::render_()
  -> Scene::update()
       -> View::update()              # Camera, input, frustum planes
       -> TerrainManager::update()    # LOD selection per terrain object
  -> Scene::render()
       -> Renderer::render()
            -> processEntities()      # Batch by shader type
            -> TerrainRenderer        # Terrain with instancing
            -> WaterRenderer          # Reflection/refraction framebuffers
            -> SkyRenderer            # Skybox cubemap
            -> VaoRenderer            # General entities
            -> GuiRenderer            # 2D overlay
            -> PostProcessor          # Atmospheric effects
```

### Key Class Hierarchy

- **Drawable** - base renderable (meshes, textures, model matrix, shader type)
  - **Light** - extends Drawable with color/ambient/diffuse/specular
  - **TerrainTile** - extends Drawable with generation attributes
- **Shader** (abstract) - `setSceneUniforms()` / `setDrawableUniforms()`
  - Implementations: TerrainInstanceShader, TerrainPlaneShader, WaterShader, SkyBoxShader, SunShader, GuiShader, PostProcessingShader, DebugShader, TessellationShader
- **TerrainObject** (abstract) - `update(View*, TerrainObjectRenderData*)`
  - CdlodTree, Planet, EndlessPlane, TerrainChunkTree
- **CdlodTreeImplementation** (abstract) - strategy for LOD tree creation
  - PlanetCdlodImplementation (6 cube faces), PlaneCdlodImplementation

### Terrain / CDLOD System

Quad-tree LOD selection based on camera distance. TerrainNode_ nodes are lazily created. Heightmaps are generated via Perlin noise (fractional Brownian motion) and cached as GPU textures for vertex shader displacement. Mesh instancing renders LOD-level base meshes efficiently.

### Data Flow

Camera position -> View frustum culling -> TerrainManager selects visible TerrainObjects -> CdlodTree::lodSelect() picks quad-tree nodes per LOD level -> instance data + heightmap textures -> TerrainRenderer draws instanced meshes

## Coding Conventions

- **Style:** LLVM-based clang-format, 4-space indent, 120 char line limit
- **Classes:** PascalCase (`TerrainManager`, `CdlodTree`)
- **Members:** snake_case with trailing underscore (`modelMatrices_`, `lodLevel_`)
- **Private methods:** trailing underscore (`initDrawable_()`, `render_()`)
- **Constants:** UPPER_CASE (`DEFAULT_AMBIENT`, `SHADER_TYPE_DEFAULT`)
- **Header guards:** `#ifndef FILENAME_H`
- **No C++ namespaces** - organization via directory structure
- **No smart pointers** - manual memory management with raw `new`/`delete`
- **Error handling:** `fprintf(stderr, ...)` warnings, minimal exceptions
- **Threading:** global `g_threadPool` (ThreadPool) for background work

## Globals (include/global.hpp)

- `g_deltaTime` - frame delta time
- `g_currentFrameTime` - current frame timestamp
- `g_triangleCount` - triangles rendered per frame
- `g_debugPolygonMode` - wireframe debug toggle
- `g_threadPool` - global thread pool instance
