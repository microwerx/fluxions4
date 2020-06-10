#include <fluxions4_vulkan_context.hpp>
#include <fluxions4_vulkan_vertex.hpp>


namespace Fluxions {
	std::array<VkVertexInputBindingDescription, 1> VulkanVertex::GetBindingDescriptions() {
		//uint32_t             binding;
		//uint32_t             stride;
		//VkVertexInputRate    inputRate;
		std::array<VkVertexInputBindingDescription, 1> vibds;
		vibds[0] = { 0, sizeof(VulkanVertex), VK_VERTEX_INPUT_RATE_VERTEX };
		return vibds;
	}

	
	std::array<VkVertexInputAttributeDescription, 4> VulkanVertex::GetAttributeDescriptions() {
		//uint32_t    location;
		//uint32_t    binding;
		//VkFormat    format;
		//uint32_t    offset;
		std::array<VkVertexInputAttributeDescription, 4> viads;
		viads[0] = { 0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanVertex, position) };
		viads[1] = { 1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanVertex, normal) };
		viads[2] = { 2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanVertex, color) };
		viads[3] = { 3, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VulkanVertex, texcoord) };
		return viads;
	}
}