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

		std::vector<VulkanSurface> cubeSurfaces{
			{ 4, 1,  0, 0 },	// front
			{ 4, 1,  4, 0 },	// back
			{ 4, 1,  8, 0 },	// left
			{ 4, 1, 12, 0 },	// right
			{ 4, 1, 16, 0 },	// top
			{ 4, 1, 20, 0 },	// bottom
		};

		std::vector<uint32_t> cubeIndices{
			0, 1, 2, 3,
			4, 5, 6, 7,
			8, 9, 10, 11,
			12, 13, 14, 15,
			16, 17, 18, 19,
			20, 21, 22, 23
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

	void CreateCube(VulkanMesh& mesh) {
		mesh.init();

		mesh.resizeVertices(24);
		mesh.updateVertexData(&cubeVertices[0], 0, cubeVertices.size());
		mesh.resizeIndices(cubeIndices.size());
		mesh.updateIndexData(&cubeIndices[0], 0, cubeIndices.size());

		for (auto& s : cubeSurfaces) {
			mesh.drawSurface(s);
		}

		mesh.copyToBuffer();
	}
}