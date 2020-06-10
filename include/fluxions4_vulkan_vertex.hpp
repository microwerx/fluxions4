#ifndef FLUXIONS4_VULKAN_VERTEX_HPP
#define FLUXIONS4_VULKAN_VERTEX_HPP

#include <array>

namespace Fluxions {
	struct VulkanVertex {
		FxVector3f position;
		FxVector3f normal;
		FxVector3f color;
		FxVector3f texcoord;

		static constexpr uint32_t POSITION = 0;
		static constexpr uint32_t NORMAL = 1;
		static constexpr uint32_t COLOR = 2;
		static constexpr uint32_t TEXCOORD = 3;

		static std::array<VkVertexInputBindingDescription, 1> GetBindingDescriptions();
		static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions();
	};
}

#endif
