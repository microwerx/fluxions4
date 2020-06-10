#include <fluxions4_simple_shapes.hpp>
#include <fluxions4_stdcxx.hpp>

namespace Fluxions {
	namespace {
		std::vector<VulkanVertex> vertices{
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
			{ { +1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { +0.0f, +1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.5f, 1.0f, 0.0f } },
		};

		std::vector<VulkanVertex> cubeVertices{
			// front
			{ { -1.0f, -1.0f, +1.0f }, { +0.0f, +0.0f, +1.0f }, { 0.0f,  0.0f,  1.0f }, { 0.0f,0.0f,0.0f } },  // point blue
			{ { +1.0f, -1.0f, +1.0f }, { +0.0f, +0.0f, +1.0f }, { 1.0f,  0.0f,  1.0f }, { 1.0f,0.0f,0.0f } },  // point magenta
			{ { -1.0f, +1.0f, +1.0f }, { +0.0f, +0.0f, +1.0f }, { 0.0f,  1.0f,  1.0f }, { 1.0f,1.0f,0.0f } },  // point cyan
			{ { +1.0f, +1.0f, +1.0f }, { +0.0f, +0.0f, +1.0f }, { 1.0f,  1.0f,  1.0f }, { 0.0f,1.0f,0.0f } },  // point white
			//  back
			{ { +1.0f, -1.0f, -1.0f }, { +0.0f, +0.0f, -1.0f }, { 1.0f,  0.0f,  0.0f }, { 0.0f,0.0f,0.0f } },  // point red
			{ { -1.0f, -1.0f, -1.0f }, { +0.0f, +0.0f, -1.0f }, { 0.0f,  0.0f,  0.0f }, { 1.0f,0.0f,0.0f } },  // point black
			{ { +1.0f, +1.0f, -1.0f }, { +0.0f, +0.0f, -1.0f }, { 1.0f,  1.0f,  0.0f }, { 1.0f,1.0f,0.0f } },  // point yellow
			{ { -1.0f, +1.0f, -1.0f }, { +0.0f, +0.0f, -1.0f }, { 0.0f,  1.0f,  0.0f }, { 0.0f,1.0f,0.0f } },  // point green
			//  right
			{ { +1.0f, -1.0f, +1.0f }, { +1.0f, +0.0f, +0.0f }, { 1.0f,  0.0f,  1.0f }, { 0.0f,0.0f,0.0f } },  // point magenta
			{ { +1.0f, -1.0f, -1.0f }, { +1.0f, +0.0f, +0.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f,0.0f,0.0f } },  // point red
			{ { +1.0f, +1.0f, +1.0f }, { +1.0f, +0.0f, +0.0f }, { 1.0f,  1.0f,  1.0f }, { 1.0f,1.0f,0.0f } },  // point white
			{ { +1.0f, +1.0f, -1.0f }, { +1.0f, +0.0f, +0.0f }, { 1.0f,  1.0f,  0.0f }, { 0.0f,1.0f,0.0f } },  // point yellow
			//  left
			{ { -1.0f, -1.0f, -1.0f }, { -1.0f, +0.0f, +0.0f }, { 0.0f,  0.0f,  0.0f }, { 0.0f,0.0f,0.0f } },  // point black
			{ { -1.0f, -1.0f, +1.0f }, { -1.0f, +0.0f, +0.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f,0.0f,0.0f } },  // point blue
			{ { -1.0f, +1.0f, -1.0f }, { -1.0f, +0.0f, +0.0f }, { 0.0f,  1.0f,  0.0f }, { 1.0f,1.0f,0.0f } },  // point green
			{ { -1.0f, +1.0f, +1.0f }, { -1.0f, +0.0f, +0.0f }, { 0.0f,  1.0f,  1.0f }, { 0.0f,1.0f,0.0f } },  // point cyan
			//  top
			{ { -1.0f, +1.0f, +1.0f }, { +0.0f, +1.0f, +0.0f }, { 0.0f,  1.0f,  1.0f }, { 0.0f,0.0f,0.0f } },  // point cyan
			{ { +1.0f, +1.0f, +1.0f }, { +0.0f, +1.0f, +0.0f }, { 1.0f,  1.0f,  1.0f }, { 1.0f,0.0f,0.0f } },  // point white
			{ { -1.0f, +1.0f, -1.0f }, { +0.0f, +1.0f, +0.0f }, { 0.0f,  1.0f,  0.0f }, { 1.0f,1.0f,0.0f } },  // point green
			{ { +1.0f, +1.0f, -1.0f }, { +0.0f, +1.0f, +0.0f }, { 1.0f,  1.0f,  0.0f }, { 0.0f,1.0f,0.0f } },  // point yellow
			//  bottom
			{ { -1.0f, -1.0f, -1.0f }, { +0.0f, -1.0f, +0.0f }, { 0.0f,  0.0f,  0.0f }, { 0.0f,0.0f,0.0f } },  // point black
			{ { +1.0f, -1.0f, -1.0f }, { +0.0f, -1.0f, +0.0f }, { 1.0f,  0.0f,  0.0f }, { 1.0f,0.0f,0.0f } },  // point red
			{ { -1.0f, -1.0f, +1.0f }, { +0.0f, -1.0f, +0.0f }, { 0.0f,  0.0f,  1.0f }, { 1.0f,1.0f,0.0f } },  // point blue
			{ { +1.0f, -1.0f, +1.0f }, { +0.0f, -1.0f, +0.0f }, { 1.0f,  0.0f,  1.0f }, { 0.0f,1.0f,0.0f } },  // point magenta
		};

		const float vVertices[] = {
			// front
			-1.0f, -1.0f, +1.0f, // point blue
			+1.0f, -1.0f, +1.0f, // point magenta
			-1.0f, +1.0f, +1.0f, // point cyan
			+1.0f, +1.0f, +1.0f, // point white
			// back
			+1.0f, -1.0f, -1.0f, // point red
			-1.0f, -1.0f, -1.0f, // point black
			+1.0f, +1.0f, -1.0f, // point yellow
			-1.0f, +1.0f, -1.0f, // point green
			// right
			+1.0f, -1.0f, +1.0f, // point magenta
			+1.0f, -1.0f, -1.0f, // point red
			+1.0f, +1.0f, +1.0f, // point white
			+1.0f, +1.0f, -1.0f, // point yellow
			// left
			-1.0f, -1.0f, -1.0f, // point black
			-1.0f, -1.0f, +1.0f, // point blue
			-1.0f, +1.0f, -1.0f, // point green
			-1.0f, +1.0f, +1.0f, // point cyan
			// top
			-1.0f, +1.0f, +1.0f, // point cyan
			+1.0f, +1.0f, +1.0f, // point white
			-1.0f, +1.0f, -1.0f, // point green
			+1.0f, +1.0f, -1.0f, // point yellow
			// bottom
			-1.0f, -1.0f, -1.0f, // point black
			+1.0f, -1.0f, -1.0f, // point red
			-1.0f, -1.0f, +1.0f, // point blue
			+1.0f, -1.0f, +1.0f  // point magenta
		};

		const float vColors[] = {
			// front
			0.0f,  0.0f,  1.0f, // blue
			1.0f,  0.0f,  1.0f, // magenta
			0.0f,  1.0f,  1.0f, // cyan
			1.0f,  1.0f,  1.0f, // white
			// back
			1.0f,  0.0f,  0.0f, // red
			0.0f,  0.0f,  0.0f, // black
			1.0f,  1.0f,  0.0f, // yellow
			0.0f,  1.0f,  0.0f, // green
			// right
			1.0f,  0.0f,  1.0f, // magenta
			1.0f,  0.0f,  0.0f, // red
			1.0f,  1.0f,  1.0f, // white
			1.0f,  1.0f,  0.0f, // yellow
			// left
			0.0f,  0.0f,  0.0f, // black
			0.0f,  0.0f,  1.0f, // blue
			0.0f,  1.0f,  0.0f, // green
			0.0f,  1.0f,  1.0f, // cyan
			// top
			0.0f,  1.0f,  1.0f, // cyan
			1.0f,  1.0f,  1.0f, // white
			0.0f,  1.0f,  0.0f, // green
			1.0f,  1.0f,  0.0f, // yellow
			// bottom
			0.0f,  0.0f,  0.0f, // black
			1.0f,  0.0f,  0.0f, // red
			0.0f,  0.0f,  1.0f, // blue
			1.0f,  0.0f,  1.0f  // magenta
		};

		const float vNormals[] = {
			// front
			+0.0f, +0.0f, +1.0f, // forward
			+0.0f, +0.0f, +1.0f, // forward
			+0.0f, +0.0f, +1.0f, // forward
			+0.0f, +0.0f, +1.0f, // forward
			// back
			+0.0f, +0.0f, -1.0f, // backbard
			+0.0f, +0.0f, -1.0f, // backbard
			+0.0f, +0.0f, -1.0f, // backbard
			+0.0f, +0.0f, -1.0f, // backbard
			// right
			+1.0f, +0.0f, +0.0f, // right
			+1.0f, +0.0f, +0.0f, // right
			+1.0f, +0.0f, +0.0f, // right
			+1.0f, +0.0f, +0.0f, // right
			// left
			-1.0f, +0.0f, +0.0f, // left
			-1.0f, +0.0f, +0.0f, // left
			-1.0f, +0.0f, +0.0f, // left
			-1.0f, +0.0f, +0.0f, // left
			// top
			+0.0f, +1.0f, +0.0f, // up
			+0.0f, +1.0f, +0.0f, // up
			+0.0f, +1.0f, +0.0f, // up
			+0.0f, +1.0f, +0.0f, // up
			// bottom
			+0.0f, -1.0f, +0.0f, // down
			+0.0f, -1.0f, +0.0f, // down
			+0.0f, -1.0f, +0.0f, // down
			+0.0f, -1.0f, +0.0f  // down
		};
	}

	VulkanMesh CreateCube() {
		VulkanMesh mesh;

		mesh.resize(24);
		mesh.update(&cubeVertices[0], 0, cubeVertices.size());

		return mesh;
	}
}