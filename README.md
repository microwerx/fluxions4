Fluxions 4.0
============

4<sup>th</sup> Generation Research 3D Graphics Engine for Vulkan.


Goals
-----

* A 3D Engine in C++20, made as simple as possible
* Generously licensed under MIT and GPLv3 for others to use
* Good for visualizations and games
* Easy to learn


Design
------

The main `Vulkan` classes are organized as follows:

- `VulkankContext` is used to manage the `VkInstance`, `VkPhysicalDevice`, `VkDevice`, and `VkQueue`
- `VulkanRenderer` is used to draw to the swapchain or for render to texture
- `VulkanPipeline` is used to pair a shader with the draw behavior
- `VulkanDescriptors` is used to configure the inputs and outputs to the shaders
- `VulkanMesh` is used to draw a mesh


### Helper Classes

- `VulkanBuffer` manages an image or vertex buffer
- `VulkanImage` manages an image and associated view

### Common Patterns

The `init` function initializes the object and the `kill` function frees resources used by the object. An object that has already been initialized will be `kill`ed and reinitialized. The destructor will call the `kill` function. Factory functions starting with `Make`... will create commonly used objects.

Sample Use
----------

```C++
using namespace Fluxions;

VulkanContext vkcontext;
vkcontext.init(1280, 720);

VulkanRenderer vkscreen;
vkscreen.init(VulkanRendererMode::SWAPCHAIN_MAILBOX);

map<int, std::vector<uint8_t>> shaders;
shaders[0] = FxLoadBinary("vert.spv");
shaders[1] = FxLoadBinary("frag.spv");
VulkanPipeline plTriStrip;
plTriStrip.setVertShader(shaders[0]);
plTriStrip.setFragShader(shaders[1]);
plTriStrip.init({
  PipelineTopology::TRIANGLE_STRIP,
  PipelineCullMode::CULL_MODE_BACK,
  PipelineDepthMode::DEPTH_LESS,
  PipelineBlendMode::NO_BLEND
}, {0, 1});

VulkanDescriptors dCubeObject;
dCubeObject.ubo.vert.modelviewMatrix = MakeIdentity();
dCubeObject.ubo.frag.Kd = Colors::Red;
dCubeObject.tex0 = texBricks;

VulkanMesh cube = MakeCube();

vkscreen.beginFrame();

plTriStrip.use(dCubeObject);
cube.draw();

vkscreen.endFrame();
vkscreen.presentFrame();
```


TODO
----

- [ ] Context
  - [x] Option use of Vulkan Validation Layer 
- [ ] Static Mesh
- [ ] Render Config
  - [ ] Dynamic loading of SPV files
- [ ] Use SQLite to store data & settings
- [ ] Use Dear ImGUI for user interface
- [ ] Math Library Tests
