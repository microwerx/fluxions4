#ifndef FLUXIONS_VULKAN_BUFFER_HPP
#define FLUXIONS_VULKAN_BUFFER_HPP

#include <fluxions_vulkan_context.hpp>

namespace Fluxions {
	class VulkanBuffer {
	public:
		VulkanBuffer();
		~VulkanBuffer();

		bool init(VulkanContext* context, VkDeviceSize allocationSize, VkBufferUsageFlags usage);
		void kill();

		void copyToMap(size_t offset, void* src, size_t size);
	private:
		VkBuffer buffer_{ nullptr };
		VkDeviceMemory memory_{ nullptr };
		uint8_t* map_{ nullptr };
		VulkanContext* context_{ nullptr };
	};
}

#endif
