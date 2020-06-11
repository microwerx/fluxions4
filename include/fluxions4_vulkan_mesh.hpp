#ifndef FLUXIONS_VULKAN_MESH_HPP
#define FLUXIONS_VULKAN_MESH_HPP

#include <fluxions4_gte_base.hpp>
#include <fluxions4_vulkan_buffer.hpp>
#include <fluxions4_vulkan_vertex.hpp>
#include <bitset>

namespace Fluxions {
	struct VulkanSurface {
		uint32_t vertexCount;
		uint32_t instanceCount;
		uint32_t firstVertex;
		uint32_t firstInstance;
	};


	class VulkanMesh {
	public:
		VulkanMesh();
		~VulkanMesh();

		void kill();

		// resizes the host vertex array
		void resize(size_t count);

		// updates the memory buffer with new vertices
		void update(VulkanVertex* newVertices, size_t start, size_t count);

		// adds a surface to render with this mesh
		void drawSurface(VulkanSurface surface);

		// removes all surfaces
		void clearSurfaces();

		// updates the Vulkan context with the new memory
		void copyToBuffer(VulkanContext& context);

		// draws to Vulkan command buffer
		void drawToCommandBuffer(VkCommandBuffer commandBuffer);

	private:
		std::vector<VulkanVertex> vertices;
		std::vector<VulkanSurface> surfaces;

		static constexpr int DIRTY_FLAG = 1;
		static constexpr int INIT_FLAG = 2;

		std::bitset<32> flags;
		VulkanBuffer vbo;
	};
}

#endif
