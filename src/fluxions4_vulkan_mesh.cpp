#include <fluxions4_vulkan_mesh.hpp>
#include <hatchetfish.hpp>


namespace Fluxions {


	VulkanMesh::VulkanMesh() {}


	VulkanMesh::~VulkanMesh() {}


	void VulkanMesh::resize(size_t count) {
		vertices.resize(count);
	}


	void VulkanMesh::update(VulkanVertex* newVertices, size_t start, size_t count) {
		if (start + count > vertices.size()) {
			HFLOGERROR("mesh size is smaller than input data");
			return;
		}

		memcpy(&vertices[start], newVertices, count * sizeof(VulkanVertex));
		flags.set(DIRTY_FLAG);
	};


	void VulkanMesh::drawSurface(VulkanSurface surface) {
		surfaces.push_back(surface);
	}


	void VulkanMesh::clearSurfaces() {
		surfaces.clear();
	}


	void VulkanMesh::copyToBuffer(VulkanContext& context) {
		if (!flags.test(DIRTY_FLAG)) return;

		size_t allocationSize = vertices.size() * sizeof(VulkanVertex);

		if (!flags.test(INIT_FLAG)) {
			if (vbo.init(&context, allocationSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT))
				flags.set(INIT_FLAG);
			else
				HFLOGERROR("Unable to initialize VBO");
		}

		if (flags.test(INIT_FLAG)) {
			vbo.copyToMap(0, &vertices[0], allocationSize);
			flags.reset(DIRTY_FLAG);
		}
	}


	void VulkanMesh::drawToCommandBuffer(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vbo.buffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		for (auto& s : surfaces) {
			//vkCmdDraw(commandBuffer, s.vertexCount, s.instanceCount, s.firstVertex, s.firstInstance);
		}
		//vkCmdDraw(commandBuffer, 4, 1, 0, 0);
	}
}