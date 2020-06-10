#ifndef FLUXIONS_VULKAN_MESH_HPP
#define FLUXIONS_VULKAN_MESH_HPP

#include <fluxions4_gte_base.hpp>
#include <fluxions4_vulkan_buffer.hpp>
#include <bitset>

namespace Fluxions {
	struct VulkanVertex {
		FxVector3f position;
		FxVector3f normal;
		FxVector3f color;
		FxVector3f texCoord;
	};


	class VulkanMesh {
	public:
		VulkanMesh();
		~VulkanMesh();

		// resizes the host vertex array
		void resize(size_t count);

		// updates the memory buffer with new vertices
		void update(VulkanVertex* newVertices, size_t start, size_t count);

		// updates the Vulkan context with the new memory
		void updateVulkan(VulkanContext& context);
	private:
		std::vector<VulkanVertex> vertices;

		static constexpr int DIRTY_FLAG = 1;
		static constexpr int INIT_FLAG = 2;

		std::bitset<32> flags;
		VulkanBuffer vbo;
	};
}

#endif
